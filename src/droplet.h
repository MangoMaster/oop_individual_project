#ifndef DROPLET_H_
#define DROPLET_H_

#include <string>

namespace DMFB
{
/**
 * @brief Droplet (name)
 * 
 */
class Droplet
{
  public:
    /*****************constructor and destructor********************/
    Droplet(const std::string &m = "")
        : name(m){};
    ~Droplet(){};
    Droplet &operator=(const Droplet &right)
    {
        this->name = right.name;
        this->sequenceNum = right.sequenceNum;
        return *this;
    }

    /*********************getter**********************/
    const std::string &getName() const
    {
        return this->name;
    }
    int getSequenceNum() const
    {
        return this->sequenceNum;
    }

    /*********************setter**********************/
    void setName(const std::string &m)
    {
        name = m;
    }

    /******************friend**********************/
    friend class Profile;

  private:
    std::string name;
    int sequenceNum = -1; // used by class Profile to identify
};

}; // namespace DMFB

#endif // DROPLET_H_