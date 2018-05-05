#ifndef PROFILE_H_
#define PROFILE_H_

#include <string>
#include <vector>
#include "droplet.h"
#include "mixer.h"
#include "detector.h"
#include "dispenser.h"

namespace DMFB
{
/**
 * @brief Data needed for DMFB solver.
 * 
 */
class Profile
{
public:
  /***************constructor and destructor***************/
  Profile() : length(-1), width(-1), time(-1){};
  ~Profile(){};

  /*********************getter********************************/
  inline const std::vector<Droplet> &getDropletVec() const;
  inline int getDropletNum() const;
  inline const std::vector<Mixer> &getMixerVec() const;
  inline int getMixerNum() const;
  inline const std::vector<Detector> &getDetectorVec() const;
  inline int getDetectorNum() const;
  inline const std::vector<Dispenser> &getDispenserVec() const;
  inline int getDispenserNum() const;
  inline int getSinkerNum() const;
  inline void getSize(int &l, int &w) const;
  inline int getSize() const;
  inline int getTime() const;
  inline int getEdgeNum() const;

  /************************setter************************/
  void addDroplet(Droplet &d);
  void addDroplet(const std::string &m = "");
  void addMixer(Mixer &m);
  void addMixer(const Droplet &d1, const Droplet &d2, int t1, const std::string &m = "");
  void addDetector(Detector &d);
  void addDetector(const Droplet &d, int t);
  void addDispenser(Dispenser &d);
  void addDispenser(const std::vector<Droplet> &d, int t = 0);
  inline void setSinkerNum(int n);
  inline void setSize(int l, int w);
  inline void setTime(int t);

  /************************finder************************/
  // given droplet, find the mixer that will accept it (if existed)
  // given sequenceNum in dropletVec, return sequenceNum in mixerVec or -1
  int findMixerOfDroplet(int dropletSequenceNum) const;
  // given droplet, find the mixer that produces it (if existed)
  // given sequenceNum in dropletVec, return sequenceNum in mixerVec or -1
  int findMixerAsDroplet(int dropletSequenceNum) const;
  // given droplet, find its detector (if existed)
  // given sequenceNum in dropletVec, return sequenceNum in dispenserVec or -1
  int findDetectorOfDroplet(int dropletSequenceNum) const;
  // given droplet, find its dispenser (if existed)
  // given sequenceNum in dropletVec, return sequenceNum in dispenserVec or -1
  int findDispenserOfDroplet(int dropletSequenceNum) const;

private:
  std::vector<Droplet> dropletVec; // 已自动包括mixer生成的droplet
  std::vector<Mixer> mixerVec;
  std::vector<Detector> detectorVec;
  std::vector<Dispenser> dispenserVec;
  int sinkerNum;
  int length;
  int width;
  int time;

  Profile(Profile &);
  Profile &operator=(Profile &);
};

/************************functions**********************/
inline const std::vector<Droplet> &Profile::getDropletVec() const
{
  return this->dropletVec;
}

inline int Profile::getDropletNum() const
{
  return this->dropletVec.size();
}

inline const std::vector<Mixer> &Profile::getMixerVec() const
{
  return this->mixerVec;
}

inline int Profile::getMixerNum() const
{
  return this->mixerVec.size();
}

inline const std::vector<Detector> &Profile::getDetectorVec() const
{
  return this->detectorVec;
}

inline int Profile::getDetectorNum() const
{
  return this->detectorVec.size();
}

inline const std::vector<Dispenser> &Profile::getDispenserVec() const
{
  return this->dispenserVec;
}

inline int Profile::getDispenserNum() const
{
  return this->dispenserVec.size();
}

inline int Profile::getSinkerNum() const
{
  return this->sinkerNum;
}

inline void Profile::getSize(int &l, int &w) const
{
  l = this->length;
  w = this->width;
}

inline int Profile::getSize() const
{
  return this->length * this->width;
}

inline int Profile::getTime() const
{
  return this->time;
}

inline int Profile::getEdgeNum() const
{
  return 2 * length + 2 * width;
}

inline void Profile::setSinkerNum(int n)
{
  sinkerNum = n;
}

inline void Profile::setSize(int l, int w)
{
  length = l;
  width = w;
}

inline void Profile::setTime(int t)
{
  time = t;
}
} // namespace DMFB

#endif // PROFILE_H_