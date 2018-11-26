#include "MovieClip.h"

MovieClip::MovieClip(uint8_t childSize, uint8_t **images, uint8_t frameCount, uint8_t FPS) : Drawable(childSize){
	my_object = (object*)RAM::malloc(sizeof(object));	
	object new_object = {};
	
	new_object.frameChangeTimes = (uint32_t*)RAM::malloc(frameCount * sizeof(uint32_t));
	if(!new_object.frameChangeTimes)
		return;
	
	new_object.images = images;
	new_object.frameCount = frameCount;
	new_object.FPS = FPS;
	copyObject(&new_object, my_object);		

	setImage(images[0]);
}

void MovieClip::update()
{	
	if(isPlaying()){
		uint8_t currentFrame = getCurrentFrame();
		uint32_t changeTime = getFrameChangeTime(currentFrame);
		//Serial.println(changeTime);
		
		if(millis() > changeTime){
			if(currentFrame < (getFrameCount() - 1)){
				currentFrame++;
				setImage(getImages()[currentFrame]);
				setCurrentFrame(currentFrame);
			}
			else{
				uint8_t repeatCount = getRepeatCount();
				if(repeatCount == 0){
					gotoAndPlay(0);
				}
				else if(repeatCount == 1){
					gotoAndStop(0);
				}
				else{
					setRepeatCount(repeatCount - 1);
					gotoAndPlay(0);
				}
			}
		}
	}
}

void MovieClip::playMovie(){
	if(isPlaying())
		return;
	
	uint8_t remainingFrameCount = getFrameCount() - getCurrentFrame();
	uint32_t frame_times[remainingFrameCount];
	uint32_t time = millis() - getPassedTime(); // The time that last frame has been shown
	uint8_t FPS = getFPS();
	
	
	for(uint8_t i = 0; i < remainingFrameCount; i++){
		//RAM::write32(&frameChangeTimes[i], (i + 1) * (1000 / FPS) + time);
		//Serial.println(RAM::read32(&frameChangeTimes[i]));
		frame_times[i] = (i + 1) * (1000 / FPS) + time;
	}
	
	uint32_t *frameChangeTimes = (uint32_t*)RAM::readPtr(&my_object->frameChangeTimes);
	RAM::writeArray(&frameChangeTimes[getCurrentFrame()], frame_times, remainingFrameCount * sizeof(uint32_t));
	setPassedTime(0);
	setPlaying(true);
}

void MovieClip::play(){
	setRepeatCount(0);
	playMovie();
}

void MovieClip::playFor(uint8_t count){
	setRepeatCount(count);
	playMovie();
}

void MovieClip::playOnce(){
	setRepeatCount(1);
	playMovie();
}

void MovieClip::stop(){
	int32_t remainingTime = getFrameChangeTime(getCurrentFrame()) - millis();
	if(remainingTime <= 0){
		setPassedTime(0);
		setPlaying(false);
		return;
	}
	
	setPassedTime(1000 / getFPS() - remainingTime);
	setPlaying(false);
}

void MovieClip::gotoAndPlay(uint8_t frame){
	if(frame > getFrameCount())
		return;
	
	setPassedTime(0);
	setCurrentFrame(frame);
	setPlaying(false);
	setImage(getImages()[frame]);
	play();
}

void MovieClip::gotoAndStop(uint8_t frame){
	if(frame > getFrameCount())
		return;
	
	setPassedTime(0);
	setCurrentFrame(frame);
	setPlaying(false);
	setImage(getImages()[frame]);
}

void MovieClip::setCurrentFrame(uint8_t value){
	RAM::write(&my_object->currentFrame, value);
}

void MovieClip::setPassedTime(uint32_t time){
	RAM::write32(&my_object->passedTime, time);
}

void MovieClip::setPlaying(uint8_t value){
	RAM::write(&my_object->isPlaying, value);
}

void MovieClip::setRepeatCount(uint8_t value){
	RAM::write(&my_object->repeatCount, value);
}

uint8_t MovieClip::isPlaying(){
	return RAM::read(&my_object->isPlaying);
}

uint8_t MovieClip::getCurrentFrame(){
	return RAM::read(&my_object->currentFrame);
}

uint32_t MovieClip::getFrameChangeTime(uint8_t frame){
	uint32_t *frameChangeTime = (uint32_t*)RAM::readPtr(&my_object->frameChangeTimes);
	return RAM::read32(&frameChangeTime[frame]);
}

uint8_t MovieClip::getFPS(){
	return RAM::read(&my_object->FPS);
}

uint8_t MovieClip::getFrameCount(){
	return RAM::read(&my_object->frameCount);
}

uint8_t MovieClip::getRepeatCount(){
	return RAM::read(&my_object->repeatCount);
}

uint32_t MovieClip::getPassedTime(){
	return RAM::read32(&my_object->passedTime);
}

uint8_t** MovieClip::getImages(){
	return (uint8_t**)RAM::readPtr(&my_object->images);
}


void MovieClip::getType(){
	
}



void MovieClip::copyObject(object* obj1, object* obj2){ //obj1 is to be copied
	RAM::writeArray(obj2, obj1, sizeof(object));
}

void MovieClip::loadObject(object* obj1, object* obj2){
	RAM::readArray(obj2, obj1, sizeof(object));
}