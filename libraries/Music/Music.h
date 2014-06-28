#ifndef MDLIB_MUSIC_H
#define MDLIB_MUSIC_H
#include <Arduino.h>

namespace mdlib {
  bool LED = false;
  // Structure for holding note / duration pairs
  struct music_note {
    long frequency;     // integer frequency
    int beats;
  };
  
  class Tune {
  public:
    Tune(): current_note_(0){}
    ~Tune(){};

    // Define the Tune by setting beat duration and note data.
    void SetBeatDuration(int duration) { beat_duration_ = duration; }
    int BeatDuration() { return beat_duration_; }

    void SetNotes(music_note* notes, int length) {
      notes_ = notes;
      length_ = length;
    }

    bool Start() {
      current_note_ = 0;
      if (current_note_ < length_) {
	current_frequency_ = (notes_ + current_note_)->frequency;
	current_duration_ = (notes_ + current_note_)->beats * beat_duration_;
      }
      else {
	current_frequency_ = 0;
	current_duration_ = 0;
      }
      return current_note_ < length_;
    }

    // advance to the next note. Returns false if Tune is done.
    bool NextNote() {
      if (current_note_ < length_)
	current_note_++;

      LED = !LED;
      digitalWrite(13, LED?HIGH:LOW);
      if (current_note_ < length_) {
	current_frequency_ = (notes_ + current_note_)->frequency;
	current_duration_ = (notes_ + current_note_)->beats * beat_duration_;
      }
      else {
	current_frequency_ = 0;
	current_duration_ = 0;
      }
      /*
      Serial.print("NextNote = ");
      Serial.print(current_note_);
      Serial.print(", freq: ");
      Serial.print(current_frequency_);
      Serial.print(", duration: ");
      Serial.print(current_duration_);
      Serial.print(", return ");
      Serial.println( current_note_ < length_ ? "TRUE" : "FALSE");
      */
      return current_note_ < length_;
    }

    // frequency of current note.
    long NoteFrequency() {
      return current_frequency_;
    }

    // duration of current note in milliseconds.
    int NoteDuration() {
      return current_duration_;
    }
    
  private:
    int current_note_;
    long current_frequency_;
    int current_duration_; 

    int beat_duration_; // milliseconds
    music_note* notes_;
    int length_;
  };

  class Player {
  public:
  Player() : playing_(false), tune_(0), note_start_(0), ms_(0) {
    }
    virtual ~Player(){};

    void SetTune(Tune* tune) { tune_ = tune; }
    void SetTime(unsigned long ms) { ms_ = ms; }
    void Play(unsigned long ms) {
      ms_ = ms;
      if (tune_) {
	playing_ = tune_->Start();
	note_start_ = ms_;
	SetFrequency(tune_->NoteFrequency());
      }
    }

    void Stop() {
      playing_ = false;
      SetFrequency(0);
    }

    // Call Update() from loop() in sketch.
    void Update(unsigned long ms) {
      ms_ = ms;
      if (playing_) {
        long elapsed = ms_ - note_start_;
	if (elapsed >= tune_->NoteDuration()) {
	  playing_ = tune_->NextNote();
	  note_start_ = ms_;
	  SetFrequency(tune_->NoteFrequency());
	}
      }
    }

  protected:
    virtual void SetFrequency(long freq) = 0;
  private:
    bool playing_;
    Tune* tune_;
    unsigned long note_start_;
    unsigned long ms_;
  };

} // namespace mdlib

#endif // MDLIB_MUSIC_H
