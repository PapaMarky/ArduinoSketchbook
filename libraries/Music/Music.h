#ifndef MDLIB_MUSIC_H
#define MDLIB_MUSIC_H
#include <Arduino.h>

namespace mdlib {

  // Structure for holding note / duration pairs
  struct music_note {
    int frequency;     // integer frequency
    int beats;
  };
  
  class Tune {
  public:
    Tune(): current_note_(0){}
    ~Tune(){};

    // Define the Tune by setting beat duration and note data.
    void SetBeatDuration(uint8_t duration) { beat_duration_ = duration; }
    uint8_t BeatDuration() { return beat_duration_; }

    void SetNotes(music_note* notes, int length) {
      notes_ = notes;
      length_ = length;
    }

    bool Start() {
      current_note_ = 0;
      return current_note_ < length_;
    }

    // advance to the next note. Returns false if Tune is done.
    bool NextNote() {
      if (current_note_ < length_)
	current_note_++;
      /*
      Serial.print("NextNote = ");
      Serial.print(current_note_);
      Serial.print(", return ");
      Serial.println( current_note_ < length_ ? "TRUE" : "FALSE");
      */
      return current_note_ < length_;
    }

    // frequency of current note.
    int NoteFrequency() {
      if (current_note_ < length_)
	return notes_[current_note_].frequency;

      return 0;
    }

    // duration of current note in milliseconds.
    int NoteDuration() {
      if (current_note_ < length_)
	return notes_[current_note_].beats * beat_duration_;

      return 0;
    }
    
  private:
    int current_note_;

    uint8_t beat_duration_; // milliseconds
    music_note* notes_;
    int length_;
  };

  class Player {
  public:
    Player() : playing_(false), tune_(0) {
    }
    virtual ~Player(){};

    void SetTune(Tune* tune) { tune_ = tune; }

    void Play() {
      if (tune_) {
	playing_ = tune_->Start();
	note_start_ = millis();
	SetFrequency(tune_->NoteFrequency());
      }
    }

    void Stop() {
      playing_ = false;
      SetFrequency(0);
    }

    // Call Update() from loop() in sketch.
    void Update() {
      if (playing_) {
        long elapsed = millis() - note_start_;
	if (elapsed >= tune_->NoteDuration()) {
	  playing_ = tune_->NextNote();
	  note_start_ = millis();
	  SetFrequency(tune_->NoteFrequency());
	}
      }
    }

  protected:
    virtual void SetFrequency(int freq) = 0;

  private:
    bool playing_;
    Tune* tune_;
    long note_start_;
  };

} // namespace mdlib

#endif // MDLIB_MUSIC_H
