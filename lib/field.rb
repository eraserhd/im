
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
    def initialize(image_loader)
      @image_loader = image_loader
      set_position(0, 0, 11*640 - 1, 5*480 - 1)
    end

    def layer
      1
    end

    def draw(vp)
      unless @images
        @images = Array.new(5)
        0.upto(4) do |n|
          @images[n] = @image_loader.get("elevator-background-#{n}")
        end
      end
      0.upto(4).each do |j|
        left = 320 + 2*640*j
        right = left + 2*640
        left -= 320 if j == 0
        right += 320 if j == 4

        0.upto((right - left)/512).each do |jj|
          0.upto(y2/512).each do |i|
            @images[j].draw(left + 512*jj - vp.x1, 512*i - vp.y1, layer)
          end
        end
         
      end
    end
  end

  def self.load(image_loader)
    f = Field.new

    f.add(Background.new(image_loader))

    f
  end

private
  class BBox < Box
    def initialize(x1,y1,x2,y2)
      set_position(x1,y1,x2,y2)
    end
  end

end
