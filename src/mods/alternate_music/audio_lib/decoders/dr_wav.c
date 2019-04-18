// Alternate music mod for 2004 Cave Story
// Copyright © 2018 Clownacy

#include "dr_wav.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define DR_WAV_IMPLEMENTATION
#define DR_WAV_NO_STDIO

#include "libs/dr_wav.h"

#include "common.h"
#include "memory_file.h"

typedef struct DecoderData_DR_WAV
{
	unsigned char *file_buffer;
	size_t file_size;
	bool loops;
} DecoderData_DR_WAV;

typedef struct Decoder_DR_WAV
{
	DecoderData_DR_WAV *data;
	drwav *instance;
} Decoder_DR_WAV;

DecoderData_DR_WAV* Decoder_DR_WAV_LoadData(const char *file_path, bool loops, LinkedBackend *linked_backend)
{
	(void)linked_backend;

	DecoderData_DR_WAV *data = NULL;

	size_t file_size;
	unsigned char *file_buffer = MemoryFile_fopen_to(file_path, &file_size);

	if (file_buffer)
	{
		data = malloc(sizeof(DecoderData_DR_WAV));
		data->file_buffer = file_buffer;
		data->file_size = file_size;
		data->loops = loops;
	}

	return data;
}

void Decoder_DR_WAV_UnloadData(DecoderData_DR_WAV *data)
{
	if (data)
	{
		free(data->file_buffer);
		free(data);
	}
}

Decoder_DR_WAV* Decoder_DR_WAV_Create(DecoderData_DR_WAV *data, DecoderInfo *info)
{
	Decoder_DR_WAV *this = NULL;

	if (data && data->file_buffer)
	{
		drwav *instance = drwav_open_memory(data->file_buffer, data->file_size);

		if (instance)
		{
			this = malloc(sizeof(Decoder_DR_WAV));

			this->instance = instance;
			this->data = data;

			info->sample_rate = instance->sampleRate;
			info->channel_count = instance->channels;
			info->decoded_size = (unsigned long)instance->totalSampleCount * sizeof(float);
			info->format = DECODER_FORMAT_F32;
		}
	}

	return this;
}

void Decoder_DR_WAV_Destroy(Decoder_DR_WAV *this)
{
	if (this)
	{
		drwav_uninit(this->instance);
		free(this->instance);
		free(this);
	}
}

void Decoder_DR_WAV_Rewind(Decoder_DR_WAV *this)
{
	drwav_seek_to_sample(this->instance, 0);
}

unsigned long Decoder_DR_WAV_GetSamples(Decoder_DR_WAV *this, void *buffer_void, unsigned long frames_to_do)
{
	const unsigned long samples_to_do = frames_to_do * this->instance->channels;

	float *buffer = buffer_void;

	unsigned long samples_done_total = 0;

	for (unsigned long samples_done; samples_done_total != samples_to_do; samples_done_total += samples_done)
	{
		samples_done = (unsigned long)drwav_read_f32(this->instance, samples_to_do - samples_done_total, buffer + samples_done_total);

		if (samples_done < samples_to_do - samples_done_total)
		{
			if (this->data->loops)
				Decoder_DR_WAV_Rewind(this);
			else
				break;
		}
	}

	return samples_done_total / this->instance->channels;
}