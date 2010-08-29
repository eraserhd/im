
require 'guy'

class Field

  class Box
    def initialize(layer, x1, y1, x2, y2, solid)
      @layer = layer
      @x1, @y1 = x1, y1
      @x2, @y2 = x2, y2
      @solid = solid

      @x1, @x2 = @x2, @x1 if @x2 < @x1
      @y1, @y2 = @y2, @y1 if @y2 < @y1
    end

    attr_reader :layer, :x1, :y1, :x2, :y2

    def solid?; @solid; end
    def hollow?; !@solid; end

    def intersects?(b)
      return false if x2 < b.x1 || b.x2 < x1
      return false if y2 < b.y1 || b.y2 < y1
      true
    end
  end

  def initialize
    @boxes = []
  end

  def add(box)
    @boxes <<= box
  end

  def boxes_intersecting(x1, y1, x2, y2)
    test_box = Field::Box.new(-1, x1, y1, x2, y2, false)
    @boxes.select{|b| b.intersects?(test_box)}.sort{|a,b| a.layer <=> b.layer}
  end


end
