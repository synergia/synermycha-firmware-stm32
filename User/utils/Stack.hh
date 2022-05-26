#pragma once

namespace utils
{

template <typename T, int N> class Stack
{
  public:
    Stack() = default;
    void push(T elem)
    {
        if (mNrOfElems < N)
        {
            mBuf[mNrOfElems] = elem;
            ++mNrOfElems;
        }
    }

    T pop()
    {
        if (mNrOfElems > 0)
        {
            return mBuf[--mNrOfElems];
        }
        else
            return T{};
    }

    T peek() const
    {
        if (mNrOfElems > 0)
        {
            return mBuf[mNrOfElems - 1];
        }
        else
            return T{};
    }

    bool isEmpty() const
    {
        return mNrOfElems == 0;
    }

    void reset()
    {
        mNrOfElems = 0;
    }

  private:
    T mBuf[N];
    int mNrOfElems = 0;
};

}  // namespace utils
