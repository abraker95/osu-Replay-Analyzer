#ifndef LZMA_H
#define LZMA_H

static class LZMA
{
	public:
		struct DataStream
		{
			const unsigned char * inData;
			size_t inLen;

			unsigned char * outData;
			size_t outLen;
		};

	static int DecompressStreamData(DataStream* _stream);

	private:
		static size_t outputCallback(void *_ctx, const void *_buf, size_t _size);
		static int inputCallback(void *_ctx, void *_buf, size_t *_size);
};

#endif