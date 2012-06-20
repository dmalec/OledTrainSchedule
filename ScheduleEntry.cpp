// --------------------------------------------------------------------------------
// MIT license.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// --------------------------------------------------------------------------------
#include "ScheduleEntry.h"

//! Class Constructor.
ScheduleEntry::ScheduleEntry() {
  _scheduled_tstamp = 0;
  _actual_tstamp = 0;
  _hours = 0;
  _minutes = 0;
  _city_index = 0;
}

void ScheduleEntry::setScheduledTstamp(uint32_t scheduled_tstamp) {
  _scheduled_tstamp = scheduled_tstamp;
}

uint32_t ScheduleEntry::getScheduledTstamp() {
  return _scheduled_tstamp;
}
    
void ScheduleEntry::setActualTstamp(uint32_t actual_tstamp) {
  _actual_tstamp = actual_tstamp;
}

uint32_t ScheduleEntry::getActualTstamp() {
  return _actual_tstamp;
}

void ScheduleEntry::setHours(uint8_t hours) {
  _hours = hours;
}

uint8_t ScheduleEntry::getHours() {
  return _hours;
}

void ScheduleEntry::setMinutes(uint8_t minutes) {
  _minutes = minutes;
}

uint8_t ScheduleEntry::getMinutes() {
  return _minutes;
}

void ScheduleEntry::setCityIndex(uint8_t city_index) {
  _city_index = city_index;
}

uint8_t ScheduleEntry::getCityIndex() {
  return _city_index;
}
