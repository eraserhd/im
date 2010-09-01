
class Field

  class Box
    attr_reader :x1, :y1, :x2, :y2

    def solid?
      true
    end

    def intersects?(b)
      return false if x2 < b.x1 || b.x2 < x1
      return false if y2 < b.y1 || b.y2 < y1
      true
    end

    def hollow?
      !solid?
    end

  protected
    def self.layer(n)
      (class << self; self; end).instance_eval do
        define_method("layer") do
          n
        end
      end
    end

    def self.solid
      (class << self; self; end).instance_eval do
        define_method("solid?") do
          true
        end
      end
    end

    def self.hollow
      (class << self; self; end).instance_eval do
        define_method("solid?") do
          false
        end
      end
    end

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
  end

  def boxes_intersecting(x1, y1, x2, y2)
    test_box = BBox.new(x1, y1, x2, y2)
    @boxes.select{|b| b.intersects?(test_box)}.sort{|a,b| a.layer <=> b.layer}
  end

private
  class BBox < Box
    def initialize(x1,y1,x2,y2)
      set_position(x1,y1,x2,y2)
    end
  end


end
