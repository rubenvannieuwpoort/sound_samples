#include <alsa/asoundlib.h>
#include <limits.h>
#include <math.h>

// if this does not play sound, try increasing this
// ALSA seems to have warmup problems...
#define TIMES 100

int main()
{
    int err;
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;
    int frequency = 440;  // set frequency to 440 Hz
    double duration = 1.0;  // set duration to 2 seconds
    double amplitude = 0.5;  // set amplitude to 0.5
    int sample_rate = 44100;  // set sample rate to 44100 Hz
    int buf_size;
    short *buf;
    int i, j;

    // Open PCM device for playback
    if ((err = snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        printf("Error opening PCM device: %s\n", snd_strerror(err));
        return -1;
    }

    // Allocate hardware parameters object
    snd_pcm_hw_params_alloca(&params);

    // Fill the hardware parameters with default values
    snd_pcm_hw_params_any(pcm_handle, params);

    // Set hardware parameters
    snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(pcm_handle, params, 1);
    snd_pcm_hw_params_set_rate_near(pcm_handle, params, &sample_rate, 0);

    // Set period size
    frames = (sample_rate * duration);
    snd_pcm_hw_params_set_period_size_near(pcm_handle, params, &frames, 0);

    // Write hardware parameters to PCM device
    if ((err = snd_pcm_hw_params(pcm_handle, params)) < 0) {
        printf("Error setting hardware parameters: %s\n", snd_strerror(err));
        return -1;
    }

    // Allocate buffer for one period
    buf_size = frames * sizeof(short);
    buf = (short *) malloc(buf_size);
    memset(buf, 0, buf_size);

    // Generate sine wave and write to buffer
    for (i = 0; i < frames; i++) {
        double t = (double) i / sample_rate;
        double x = amplitude * sin(2 * M_PI * frequency * t);
        buf[i] = (short) (x * (double) SHRT_MAX);
    }

    // Play the buffer repeatedly
    for (j = 0; j < 88; j++) {
        if ((err = snd_pcm_writei(pcm_handle, buf, frames)) != frames) {
            printf("Error playing wave: %s\n", snd_strerror(err));
            return -1;
        }
	}

    // Close PCM device
    snd_pcm_close(pcm_handle);

    // Free buffer
    free(buf);

    return 0;
}
