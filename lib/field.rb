
require 'image_loader'

class Field

  class Box
    attr_reader :x1, :y1, :x2, :y2

    def intersects?(b)
      return false if x2 < b.x1 || b.x2 < x1
      return false if y2 < b.y1 || b.y2 < y1
      true
    end

    def draw(vp)
    end

  protected
    def set_position(x1, y1, x2, y2)
      @x1, @y1 = x1, y1
      @x2, @y2 = x2, y2

      @x1, @x2 = @x2, @x1 if @x2 < @x1
      @y1, @y2 = @y2, @y1 if @y2 < @y1
    end
  end

  def initialize
    @boxes = []
  end

  def add(box)
    @boxes <<= box
    @boxes.sort! {|a,b| b.layer <=> a.layer}
  end

  def draw(vp)
    @boxes.reverse_each do |b|
      b.draw(vp)
    end
  end

  def hit?(x, y)
    @boxes.each do |b|
      if x >= b.x1 && x <= b.x2 && y >= b.y1 && y <= b.y2
        return (b.layer % 2) == 1
      end
    end
    false
  end

  class Background <Box
    attr_reader :index

    def initialize(index, x1, x2, image)
      @index = index
      set_position(x1, 0, x2, 480)
      @image = image
    end

    def layer
      1
    end

    def draw(vp)
      @image.draw(x1 - vp.x1, y1 - vp.y1, layer)
    end
  end

  def self.load(image_loader)
    f = Field.new

    0.upto(4).each do |i|
      left = 2*640*i + 320
      right = left + 2*640 - 1
      left -= 320 if i == 0
      right += 320 if i == 4

      bg = Background.new(i, left, right, image_loader.get("elevator-background-#{i}"))
      f.add(bg)
    end

    f
  end

private
  class BBox < Box
    def initialize(x1,y1,x2,y2)
      set_position(x1,y1,x2,y2)
    end
  end

end
