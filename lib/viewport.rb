
class Viewport

  attr_reader :field_width, :field_height
  attr_reader :x1, :y1

  GUTTER = 50
  GRADUAL_MOVE_DISTANCE = 5

  def width
    640
  end
  def height
    480
  end

  def initialize(x1, y1, field_width, field_height)
    @x1 = x1
    @y1 = y1
    @width = width
    @height = height
    @field_width = field_width
    @field_height = field_height
  end

  def x2
    @x1 + @width - 1
  end

  def y2
    @y1 + @height - 1
  end

  def contains?(thing)
    return false unless thing.x >= x1 && thing.x <= x2
    return false unless thing.y >= y1 && thing.y <= y2
    true
  end

  def adjust_to_guy(guy)
    # Gradual movement
    centered_x1 = guy.x - width/2
    centered_y1 = guy.y - height/2

    if Math.sqrt((centered_x1 - x1) ** 2 + (centered_y1 - y1) ** 2) <= GRADUAL_MOVE_DISTANCE
      @x1 = centered_x1
      @y1 = centered_y1
    else
      angle = Math.atan2(centered_x1 - x1, centered_y1 - y1)
      @x1 += (GRADUAL_MOVE_DISTANCE * Math.sin(angle)).to_i
      @y1 += (GRADUAL_MOVE_DISTANCE * Math.cos(angle)).to_i
    end

    # Don't go out of bounds, no matter what (non-gradual)
    @x1 = [@x1, guy.x - GUTTER].min
    @y1 = [@y1, guy.y - GUTTER].min
    @x1 = [@x1, guy.x + GUTTER - width, 0].max
    @y1 = [@y1, guy.y + GUTTER - height, 0].max
    @x1 = [@x1, field_width-width].min
    @y1 = [@y1, field_height-height].min
  end

end
