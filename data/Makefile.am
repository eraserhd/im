
IMAGE_FILES			= elevator-background-0.bmp\
				  elevator-background-1.bmp\
				  elevator-background-2.bmp\
				  elevator-background-3.bmp\
				  elevator-background-4.bmp\
				  elevator-background-5.bmp\
				  elevator-0.bmp\
				  elevator-1.bmp\
				  elevator-2.bmp\
				  elevator-3.bmp\
				  elevator-4.bmp\
				  flipping-left-0.bmp \
				  flipping-left-10.bmp \
				  flipping-left-11.bmp \
				  flipping-left-1.bmp \
				  flipping-left-2.bmp \
				  flipping-left-3.bmp \
				  flipping-left-4.bmp \
				  flipping-left-5.bmp \
				  flipping-left-6.bmp \
				  flipping-left-7.bmp \
				  flipping-left-8.bmp \
				  flipping-left-9.bmp \
				  flipping-right-0.bmp \
				  flipping-right-10.bmp \
				  flipping-right-11.bmp \
				  flipping-right-1.bmp \
				  flipping-right-2.bmp \
				  flipping-right-3.bmp \
				  flipping-right-4.bmp \
				  flipping-right-5.bmp \
				  flipping-right-6.bmp \
				  flipping-right-7.bmp \
				  flipping-right-8.bmp \
				  flipping-right-9.bmp \
				  running-left-0.bmp \
				  running-left-10.bmp \
				  running-left-11.bmp \
				  running-left-12.bmp \
				  running-left-13.bmp \
				  running-left-1.bmp \
				  running-left-2.bmp \
				  running-left-3.bmp \
				  running-left-4.bmp \
				  running-left-5.bmp \
				  running-left-6.bmp \
				  running-left-7.bmp \
				  running-left-8.bmp \
				  running-left-9.bmp \
				  running-right-0.bmp \
				  running-right-10.bmp \
				  running-right-11.bmp \
				  running-right-12.bmp \
				  running-right-13.bmp \
				  running-right-1.bmp \
				  running-right-2.bmp \
				  running-right-3.bmp \
				  running-right-4.bmp \
				  running-right-5.bmp \
				  running-right-6.bmp \
				  running-right-7.bmp \
				  running-right-8.bmp \
				  running-right-9.bmp \
				  searching-0.bmp \
				  standing-left-0.bmp \
				  standing-right-0.bmp

SOUND_FILES			= footstep.wav\
				  elevator.wav\
				  elevator-start.wav\
				  elevator-stop.wav

pkgdata_DATA			= $(IMAGE_FILES) $(SOUND_FILES)

EXTRA_DIST			= $(IMAGE_FILES) $(SOUND_FILES)

win32-binary-dist:
	mkdir -p ../im-win32/data/
	for file in $(DISTFILES) ; do \
	  case "$$file" in \
	    *.bmp|*.wav) \
	      cp -a "$$file" ../im-win32/data/ \
	      ;; \
	  esac \
	done
