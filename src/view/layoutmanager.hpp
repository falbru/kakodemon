#ifndef VIEW_LAYOUTMANAGER_HPP_INCLUDED
#define VIEW_LAYOUTMANAGER_HPP_INCLUDED

struct Rect
{
    float x;
    float y;
    float width;
    float height;

    Rect(float x, float y, float width, float height);
};

class LayoutManager
{
  public:
    LayoutManager(float x, float y, float width, float height);

    const Rect &current() const;
    LayoutManager copy() const;

    void pad(float padding);
    void pad(float paddingY, float paddingX);
    void border(float border_size);

    void gapX(float gap_size);
    void gapY(float gap_size);

    LayoutManager sliceTop(float height);

  private:
    Rect m_current;
};

#endif
