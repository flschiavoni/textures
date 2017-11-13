all:
	gcc tobmp.c -o tobmp
	gcc towav.c -o towav
	gcc medusa_dsp_hton.c -c -lm -o medusa_dsp_hton.o
	gcc medusa_dsp_resample.c -c -lm -o medusa_dsp_resample.o
	gcc medusa_dsp_quantization.c -c -lm -o medusa_dsp_quantization.o
	gcc wav2texture.c medusa_dsp_resample.o medusa_dsp_quantization.o medusa_dsp_hton.o -o wav2texture -lm -lsndfile -g

clean:
	rm -rf tobmp
	rm -rf towav
	rm -rf wav2texture
	rm -rf medusa_dsp*o
