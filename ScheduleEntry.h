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
#ifndef ScheduleEntry_h
#define ScheduleEntry_h

#include <Arduino.h>

class ScheduleEntry {
  
  public:
    ScheduleEntry();
    
    void setScheduledTstamp(uint32_t scheduled_tstamp);
    uint32_t getScheduledTstamp();
    
    void setActualTstamp(uint32_t actual_tstamp);
    uint32_t getActualTstamp();

    void setHours(uint8_t hours);
    uint8_t getHours();

    void setMinutes(uint8_t minutes);
    uint8_t getMinutes();

    void setCityIndex(uint8_t city_index);
    uint8_t getCityIndex();

  private:
    uint32_t _scheduled_tstamp;
    uint32_t _actual_tstamp;
    uint8_t _city_index;
    uint8_t _hours;
    uint8_t _minutes;
};

#endif // ScheduleEntry_h

