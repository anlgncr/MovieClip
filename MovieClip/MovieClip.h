#ifndef MovieClip_h
#define MovieClip_h
#include "arduino.h"
#include "Drawable.h"
#include "RAM.h"

class MovieClip : public Drawable{
	public:
		MovieClip(uint8_t, uint8_t**, uint8_t, uint8_t);
		
		void update();		
		void getType();
		
		void play();
		void playFor(uint8_t);
		void playOnce();
		void gotoAndPlay(uint8_t);
		void stop();
		void gotoAndStop(uint8_t);
		
		uint8_t isPlaying();
		uint8_t getFPS();
		uint8_t getFrameCount();
		uint8_t getCurrentFrame();
		uint32_t getFrameChangeTime(uint8_t);
		uint8_t getRepeatCount();
		
		void setRepeatCount(uint8_t);
		void setPassedTime(uint32_t);
		void setPlaying(uint8_t);
		void setCurrentFrame(uint8_t);
		
	private:
		struct object{
			uint8_t FPS;
			uint8_t frameCount;
			uint8_t repeatCount;
			uint8_t currentFrame;
			uint8_t isPlaying;
			uint8_t **images;
			uint32_t *frameChangeTimes;
			uint32_t passedTime;	
		};
		object *my_object;
		void copyObject(object*, object*);
		void loadObject(object*, object*);
		void playMovie();
		uint32_t getPassedTime();
		uint8_t** getImages();
};

#endif