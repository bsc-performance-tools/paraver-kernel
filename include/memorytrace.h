#ifndef MEMORYTRACE_H_INCLUDED
#define MEMORYTRACE_H_INCLUDED
class MemoryTrace
{
  public:
    class  iterator
    {
      public:
        virtual iterator() = 0;
        virtual ~iterator() = 0;

        virtual void operator++() = 0;
        virtual void operator--() = 0;

        virtual TType getType() = 0;
        virtual TTime getTime() = 0;
      protected:

      private:

    };


    MemoryTrace();
    ~MemoryTrace();
  protected:
    struct TRecord;

  private:

};



#endif // MEMORYTRACE_H_INCLUDED
