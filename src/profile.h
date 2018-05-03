#ifndef PROFILE_H_
#define PROFILE_H_

#include <string>
#include <vector>
#include "droplet.h"
#include "mixer.h"
#include "detector.h"
#include "dispenser.h"
#include "configuration.h"

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
  Profile() : cfg(), length(0), width(0), time(0){};
  Profile(const Configuration &c) : cfg(c), length(0), width(0), time(0){};
  ~Profile(){};
  //Profile(Profile &);

  /*********************getter********************************/
  const std::vector<Droplet> &getDropletVec() const
  {
    return this->dropletVec;
  }
  const std::vector<Mixer> &getMixerVec() const
  {
    return this->mixerVec;
  }
  const std::vector<Detector> &getDetectorVec() const
  {
    return this->detectorVec;
  }
  const std::vector<Dispenser> &getDispenserVec() const
  {
    return this->dispenserVec;
  }
  int getSinkerNum() const
  {
    return this->sinkerNum;
  }
  const Configuration &getConfig() const
  {
    return this->cfg;
  }
  void getSize(int &l, int &w) const
  {
    l = length;
    w = width;
  }
  int getTime() const
  {
    return time;
  }
  int getEdgeNum() const
  {
    return 2 * length + 2 * width;
  }
  int getDropletNum() const;
  int getMixerNum() const;
  int getDetectorNum() const;
  int getDispenserNum() const;

  /************************setter************************/
  void addDroplet(const Droplet &d) // explicit
  {
    dropletVec.push_back(d);
  }
  void addDroplet(const std::string &m = "", int n = 1, int t = 0)
  {
    Droplet d(m, n, t);
    addDroplet(d);
  }
  void addDroplet(int n)
  {
    Droplet d(n);
    addDroplet(d);
  }
  void addMixer(const Mixer &m)
  {
    mixerVec.push_back(m);
  }
  void addMixer(const Droplet &d1, const Droplet &d2, int t1, const std::string &m = "", int n = 1, int t2 = 0)
  {
    Mixer mix(d1, d2, t1, m, n, t2);
    addMixer(mix);
  }
  void addMixer(const Droplet &d1, const Droplet &d2, int t1, int n)
  {
    Mixer mix(d1, d2, t1, n);
    addMixer(mix);
  }
  void addDetector(const Detector &d)
  {
    detectorVec.push_back(d);
  }
  void addDetector(const Droplet &d, int t)
  {
    Detector det(d, t);
    detectorVec.push_back(det);
  }
  void addDispenser(const Dispenser &d)
  {
    dispenserVec.push_back(d);
  }
  void addDispenser(const Droplet &d, int n = 1, int t = 0)
  {
    Dispenser dis(d, n, t);
    dispenserVec.push_back(dis);
  }
  void setSinkerNum(int n)
  {
    sinkerNum = n;
  }
  void setSize(int l, int w)
  {
    length = l;
    width = w;
  }
  void setTime(int t)
  {
    time = t;
  }

private:
  const Configuration cfg;
  std::vector<Droplet> dropletVec;
  std::vector<Mixer> mixerVec;
  std::vector<Detector> detectorVec;
  std::vector<Dispenser> dispenserVec;
  int sinkerNum;
  int length;
  int width;
  int time;

  Profile &operator=(Profile &);
};
} // namespace DMFB

#endif // PROFILE_H_