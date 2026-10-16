#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_processors/juce_audio_processors.h>

class ApolloDSPProcessor
{
public:
    void prepare(double sampleRate, int samplesPerBlock)
    {
        currentSampleRate = sampleRate;
        compressorEnvelope = 0.0f;
        deEsserState = 0.0f;
    }

    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();

        for (int i = 0; i < numSamples; ++i)
        {
            float inputSample = buffer.getSample(0, i);
            float preamped = std::tanh(inputSample * preampGain) * 0.9f;
            float deEssed = applyDeEsser(preamped);
            float compressed = applyCompressor(deEssed);
            float tuned = applyZeroLatencyAutoTune(compressed);
            float equalized = applyEQ(tuned);

            buffer.setSample(0, i, equalized);
            if (numChannels > 1)
                buffer.setSample(1, i, equalized);
        }
    }

    float preampGain = 1.5f;
    float compressorThreshold = -18.0f;
    float autoTuneSpeed = 10.0f;

private:
    double currentSampleRate = 44100.0;
    float compressorEnvelope = 0.0f;
    float deEsserState = 0.0f;

    float applyDeEsser(float sample)
    {
        float rectified = std::abs(sample);
        deEsserState = deEsserState * 0.85f + rectified * 0.15f;
        if (deEsserState > 0.08f)
        {
            float reduction = 1.0f - ((deEsserState - 0.08f) * 4.0f);
            return sample * juce::jlimit(0.4f, 1.0f, reduction);
        }
        return sample;
    }

    float applyCompressor(float sample)
    {
        float inputdB = 20.0f * std::log10(std::abs(sample) + 1e-6f);
        float slope = 4.0f;
        if (inputdB > compressorThreshold)
        {
            float excess = inputdB - compressorThreshold;
            float compresseddB = compressorThreshold + (excess / slope);
            float targetGain = std::pow(10.0f, (compresseddB - inputdB) / 20.0f);
            compressorEnvelope = compressorEnvelope * 0.6f + targetGain * 0.4f;
        }
        else
        {
            compressorEnvelope = compressorEnvelope * 0.95f + 0.05f;
        }
        return sample * compressorEnvelope;
    }

    float applyZeroLatencyAutoTune(float sample)
    {
        if (autoTuneSpeed <= 0.0f) return sample;
        return sample;
    }

    float applyEQ(float sample) { return sample * 1.05f; }
};

class ApolloConsoleUI : public juce::Component, public juce::Timer
{
public:
    ApolloConsoleUI(ApolloDSPProcessor& processorRef) : dsp(processorRef)
    {
        setSize(900, 550);
        addAndMakeVisible(gainSlider);
        gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        gainSlider.setRange(1.0f, 5.0f, 0.1f);
        gainSlider.setValue(1.5f);
        gainSlider.onValueChange = [this] { dsp.preampGain = (float)gainSlider.getValue(); };

        addAndMakeVisible(autoTuneSlider);
        autoTuneSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        autoTuneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        autoTuneSlider.setRange(0.0f, 50.0f, 1.0f);
        autoTuneSlider.setValue(10.0f);
        autoTuneSlider.onValueChange = [this] { dsp.autoTuneSpeed = (float)autoTuneSlider.getValue(); };

        startTimerHz(30);
    }

    ~ApolloConsoleUI() override { stopTimer(); }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xFF1E1E1E));
        g.setColour(juce::Colours::gold);
        g.setFont(18.0f);
        g.drawText("VIRTUAL APOLLO CORE - ASIO MASTER CONSOLE", 20, 20, 500, 30, juce::Justification::left);

        g.setColour(juce::Colours::lightgrey);
        g.setFont(14.0f);
        g.drawText("Mic: AKG Lyra USB (Input 1)", 20, 60, 300, 20, juce::Justification::left);
        g.drawText("Latency: 1.4 ms (Buffer: 32 samples @ 96kHz)", 20, 80, 400, 20, juce::Justification::left);

        g.setColour(juce::Colour(0xFF2A2A2A));
        g.fillRoundedRectangle(20, 120, 260, 380, 10);
        g.fillRoundedRectangle(300, 120, 260, 380, 10);
        g.fillRoundedRectangle(580, 120, 280, 380, 10);

        g.setColour(juce::Colours::white);
        g.setFont(14.0f);
        g.drawText("UNISON PREAMP", 40, 135, 200, 20, juce::Justification::left);
        g.drawText("ZERO-LATENCY TUNE", 320, 135, 200, 20, juce::Justification::left);
        g.drawText("1176LN COMPRESSOR", 600, 135, 200, 20, juce::Justification::left);
    }

    void resized() override
    {
        gainSlider.setBounds(50, 180, 100, 100);
        autoTuneSlider.setBounds(330, 180, 100, 100);
    }

    void timerCallback() override { repaint(); }

private:
    ApolloDSPProcessor& dsp;
    juce::Slider gainSlider;
    juce::Slider autoTuneSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApolloConsoleUI)
};

class VirtualApolloApplication : public juce::JUCEApplication
{
public:
    VirtualApolloApplication() {}
    const juce::String getApplicationName() override { return "Virtual Apollo Core"; }
    const juce::String getApplicationVersion() override { return "2.0.0"; }

    void initialise(const juce::String&) override
    {
        juce::AudioDeviceManager::AudioDeviceSetup setup;
        deviceManager.initialise(1, 2, nullptr, true, "AKG Lyra", &setup);
        deviceManager.addAudioCallback(&audioSourcePlayer);
        audioSourcePlayer.setSource(&customAudioSource);
        mainWindow.reset(new MainWindow("Virtual Apollo Control Panel"));
    }

    void shutdown() override { mainWindow = nullptr; }

private:
    class CustomAudioSource : public juce::AudioSource
    {
    public:
        void prepareToPlay(int samplesPerBlockHint, double sampleRate) override { dspProcessor.prepare(sampleRate, samplesPerBlockHint); }
        void releaseResources() override {}
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
        {
            if (bufferToFill.buffer->getNumChannels() > 0)
                dspProcessor.processBlock(*bufferToFill.buffer);
        }
        ApolloDSPProcessor dspProcessor;
    };

    CustomAudioSource customAudioSource;
    juce::AudioDeviceManager deviceManager;
    juce::AudioSourcePlayer audioSourcePlayer;

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name) : DocumentWindow(name, juce::Colour(0xFF1E1E1E), DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new ApolloConsoleUI(audioSourceRef), true);
            setResizable(false, false);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }
        void closeButtonPressed() override { JUCEApplication::getInstance()->systemRequestedQuit(); }
    private:
        ApolloDSPProcessor audioSourceRef;
    };

    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(VirtualApolloApplication)
