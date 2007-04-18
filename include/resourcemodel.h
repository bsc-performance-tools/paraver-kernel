#ifndef RESOURCEMODEL_H_INCLUDED
#define RESOURCEMODEL_H_INCLUDED


class ResourceModel
{
  public:
    ResourceModel();
    ~ResourceModel();

    bool isReady() { return ready; }

  protected:

  private:
    bool ready;
};


#endif // RESOURCEMODEL_H_INCLUDED
