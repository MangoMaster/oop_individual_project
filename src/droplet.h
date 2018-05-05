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
        : name(m), sequenceNum(-1){};
    ~Droplet(){};
    Droplet &operator=(const Droplet &right)
    {
        this->name = right.name;
        this->sequenceNum = right.sequenceNum;
    }

    /*********************getter**********************/
    const std::string &getName() const
    {
        return this->name;
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
    int sequenceNum; // used by class Profile to identify
};

}; // namespace DMFB

#endif // DROPLET_H_