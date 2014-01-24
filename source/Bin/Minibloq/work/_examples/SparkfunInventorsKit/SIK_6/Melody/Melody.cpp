#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	toneWithDelay(BuzzerPin, NOTE_C4, 1915);
	toneWithDelay(BuzzerPin, NOTE_D4, 1700);
	toneWithDelay(BuzzerPin, NOTE_E4, 1519);
	toneWithDelay(BuzzerPin, NOTE_F4, 1432);
	toneWithDelay(BuzzerPin, NOTE_G4, 1275);
	toneWithDelay(BuzzerPin, NOTE_A4, 1136);
	toneWithDelay(BuzzerPin, NOTE_B4, 1014);
	toneWithDelay(BuzzerPin, NOTE_C4, 956);
}

void loop()
{
}
