// --------------------------------------------------------------------------------
// An Arduino sketch to display a random train schedule on an OLED.
//
// MIT license.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//      **********************************************************
//      Designed for the Adafruit OLED Breakout Board - 16-bit Color 0.96" w/microSD holder
//      http://www.adafruit.com/products/684
//      **********************************************************
//
// --------------------------------------------------------------------------------
// Dependencies
// --------------------------------------------------------------------------------
// Adafruit Industries's core graphics library:
//       https://github.com/adafruit/Adafruit-GFX-Library
// Adafruit Industries's SSD1331 OLED driver library:
//       https://github.com/adafruit/Adafruit-SSD1331-OLED-Driver-Library-for-Arduino
// Adafruit Industries's DS1307 RTC library:
//       https://github.com/adafruit/RTClib
// --------------------------------------------------------------------------------
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include "ScheduleEntry.h"

//! The array of city names to choose from.
/*!
  When adding or removing cities, adjust the NUMBER_OF_CITIES to match.
  City names of 10 characters or less work best for the screen size.
*/
const int NUMBER_OF_CITIES = 20;
const char *names_of_cities[NUMBER_OF_CITIES] = {
  "ALBANY",
  "BERKELEY",
  "BOSTON",
  "BUFFALO",
  "CHICAGO",
  "DENVER",
  "DETROIT",
  "D.C.",
  "DURAND",
  "HOBOKEN",
  "L.A.",
  "MILWAUKEE",
  "NEW HAVEN",
  "NEWARK",
  "NEW YORK",
  "PROVIDENCE",
  "SEATTLE",
  "ST. LOUIS",
  "SYRACUSE",
  "TRENTON"
};

//! The current display screen.
uint8_t current_screen_index;

//! The timestamp of the last screen change.
long last_screen_change_tstamp;

//! The number of seconds to display each screen.
const uint8_t seconds_to_display_screen = 5;

//! The number of seconds to display departed entries.
const uint8_t seconds_to_display_departed_entries = 60;

//! Number of entries per screen, 6 fits nicely.
const uint8_t number_of_entries_per_screen = 6;

//! Number of screens to show.
const uint8_t number_of_screens = 3;

//! Maximum number of minutes past current time to schedule departure times.
const uint8_t minutes_until_departure = 15;

//! Percentage of late departures.
const uint8_t percent_late_departures = 30;

//! MAximum number of minutes to be late.
const long minutes_late = 10;

//! Number of total entries.
const uint8_t number_of_entries = number_of_screens * number_of_entries_per_screen;

//! The array of all destinations
ScheduleEntry schedule_entries[number_of_entries];

//! Configure the SSD1331 display to use hardware SPI pins, from the example sketch.
#define cs   10
#define rst  9
#define dc   8
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

//! Colors used by the sketch.
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0 
#define WHITE           0xFFFF

//! Mock real time clock for keeping time.
RTC_Millis rtc;

//! The current time per the real time clock.
DateTime now;

//! Perform all initialization.
void setup(void) {
  // Initialize the serial connection for debugging.
  Serial.begin(9600);
  Serial.println(F("Serial initialized"));
  
  // Use Pin 0 to seed the random number generator.
  randomSeed(analogRead(0));

  // Initialize the real time clock and time variables.
  rtc.begin(DateTime(__DATE__, __TIME__));    
  now = rtc.now();
  last_screen_change_tstamp = now.unixtime();

  // Initialize the schedule entries.
  for (uint8_t i=0; i<number_of_entries; i++) {
    generate_new_entry(i);
  }

  // Initialize the OLED.
  display.begin();
  display.fillScreen(BLACK);

  // Render page 0 of entries.
  current_screen_index = 0;
  display_current_screen();

  // Draw the bottom blue bar.
  display.fillRect(0, 56, 96, 8, BLUE);
  display.setTextColor(WHITE);

  // Draw the town name.
  display.setCursor(0, 56);
  display.print(F("BRICKVILLE"));
}

void loop(void) {
  now = rtc.now();
  static int old_minute;

  // Update the clock in the bottom banner if needed.  
  if (old_minute != now.minute()) {
    // Erase the old time.
    display.fillRect(64, 56, 32, 8, BLUE);
    
    // Draw the new time.
    display.setCursor(64, 56);
    display.setTextColor(WHITE);
    print_time(now.hour(), now.minute());

    // Update the timestamp.
    old_minute = now.minute();
  }
  
  // At the configured interval, change to the next screen of entries.
  if (now.unixtime() - last_screen_change_tstamp >= seconds_to_display_screen) {
    current_screen_index++;
    
    // Wrap around to the first screen if at the last screen.
    if (current_screen_index >= number_of_screens) {
      current_screen_index = 0;
    }
    
    display_current_screen();
    
    // Update the timestamp.
    last_screen_change_tstamp = now.unixtime();
  }

  // Regenerate entries that departed more than the configured number of minutes ago.
  for (int i=0; i<number_of_entries; i++) {
    if ((now.unixtime() > schedule_entries[i].getActualTstamp()) &&
        (now.unixtime() - schedule_entries[i].getActualTstamp() >= seconds_to_display_departed_entries)) {
      generate_new_entry(i);
    }
  }

  delay(100);
}

//! Randomly generate a new entry.
void generate_new_entry(uint8_t index) {
  // Randomly choose a scheduled departure time.
  long total_minutes = 1 + random(minutes_until_departure);

  // Calculate the hours and minutes.
  uint8_t hours = total_minutes / 60;
  uint8_t minutes = total_minutes % 60;

  // Add to the current time.
  hours += now.hour();
  minutes += now.minute();
  
  // Account for minute wrap around.
  if (minutes > 59) {
    minutes -= 60;
    hours++;
  }
  
  // Account for hours wrap around.
  if (hours > 23) {
    hours -= 24;
  }

  // Randomly choose some departures as late.
  if (random(100) > percent_late_departures) {
    schedule_entries[index].setActualTstamp(now.unixtime() + (uint32_t)(total_minutes * 60));
  } else {
    schedule_entries[index].setActualTstamp(now.unixtime() + (uint32_t)((total_minutes + random(minutes_late)) * 60));
  }
  
  // Populate the schedule entry with the new values.
  schedule_entries[index].setScheduledTstamp(now.unixtime() + (uint32_t)(total_minutes * 60));
  schedule_entries[index].setHours(hours);
  schedule_entries[index].setMinutes(minutes);
  schedule_entries[index].setCityIndex(random(NUMBER_OF_CITIES));

  // Debugging output.
  Serial.print(F("New Entry ("));
  Serial.print(index);
  Serial.print(F(") "));
  Serial.print(names_of_cities[schedule_entries[index].getCityIndex()]);
  Serial.print(F(" "));
  Serial.print(schedule_entries[index].getHours());
  Serial.print(F(":"));
  Serial.print(schedule_entries[index].getMinutes());
  Serial.print(F(" "));
  Serial.print(schedule_entries[index].getScheduledTstamp());
  Serial.print(F("/"));
  Serial.print(schedule_entries[index].getActualTstamp());
  Serial.println();
}

//! Display the current page of entries.
void display_current_screen() {
  uint8_t schedule_index;
  
  // Erase the schedule area
  display.fillRect(0, 0, 96, 56, BLACK);
  
  for (int row = 0; row < number_of_entries_per_screen; row++) {
    schedule_index = current_screen_index * number_of_entries_per_screen + row;
    
    // Display the city name in white.
    display.setTextColor(WHITE);
    display.setCursor(0, row * 8);
    display.print(names_of_cities[schedule_entries[schedule_index].getCityIndex()]);
    
    display.setCursor(64, row * 8);
    if (now.unixtime() > schedule_entries[schedule_index].getActualTstamp()) {
      // If the entry is departed, display a green DPRTD instead of time.
      display.setTextColor(GREEN);
      display.print(F("DPRTD"));
    } else {
      // If the entry is late, display the time in red, otherwise in white.
      if (now.unixtime() > schedule_entries[schedule_index].getScheduledTstamp()) {
        display.setTextColor(RED);
      } else {
        display.setTextColor(WHITE);
      }
      print_time(schedule_entries[schedule_index].getHours(), schedule_entries[schedule_index].getMinutes());
    }
  }
}

//! Utility function to print a zero padded time.
void print_time(uint8_t hours, uint8_t minutes) {
  if (hours < 10) {
    display.print('0');
  }
  display.print(hours);

  display.print(':');

  if (minutes < 10) {
    display.print('0');
  }
  display.print(minutes);
}
