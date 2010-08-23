
class Guy

  class Facing; end
  LEFT = Facing.new
  RIGHT = Facing.new

  class Activity; end
  class Standing < Activity
    def frame_count; 1; end
  end
  STANDING = Standing.new

  class Running < Activity
    def frame_count; 14; end
  end
  RUNNING = Running.new

  class Flipping < Activity
    def frame_count; 12; end
  end
  FLIPPING = Flipping.new

  class ImageSet
    class NoImage < Exception
    end

    def initialize(window)
      @images = {
        LEFT => {
          RUNNING => 0.upto(13).map{|n| Gosu::Image.new(window, "data/running-left-#{n}.png", false)},
          STANDING => [ Gosu::Image.new(window, "data/standing-left-0.png", false) ],
          FLIPPING => 0.upto(11).map{|n| Gosu::Image.new(window, "data/flipping-left-#{n}.png", false)}
        },
        RIGHT => {
          RUNNING => 0.upto(13).map{|n| Gosu::Image.new(window, "data/running-right-#{n}.png", false)},
          STANDING => [ Gosu::Image.new(window, "data/standing-right-0.png", false) ],
          FLIPPING => 0.upto(11).map{|n| Gosu::Image.new(window, "data/flipping-right-#{n}.png", false)}
        }
      }
    end

    def get(facing, activity, frame)
      raise NoImage.new("Invalid image") unless @images[facing]
      raise NoImage.new("Invalid image") unless @images[facing][activity]
      raise NoImage.new("Invalid image") unless @images[facing][activity][frame]
      @images[facing][activity][frame]
    end
  end

  def initialize(image_set)
    @facing = LEFT
    @activity = STANDING
    @frame = 0
    @image_set = image_set
  end

  attr_reader :activity, :facing, :frame

  def tick(params)
    if params[:space_pressed] && (@activity != FLIPPING)
      @activity = FLIPPING
      @frame = 0
      return
    elsif params[:left_pressed] && (@activity != RUNNING || @facing != LEFT)
      @facing = LEFT
      @activity = RUNNING
      @frame = 0
      return
    elsif params[:right_pressed] && (@activity != RUNNING || @facing != RIGHT)
      @facing = RIGHT
      @activity = RUNNING
      @frame = 0
      return
    elsif @activity == RUNNING && !params[:right_pressed] && !params[:left_pressed]
      @activity = STANDING
      @frame = 0
      return
    end

    @frame = (@frame + 1) % @activity.frame_count
  end

  def image
    @image_set.get(@facing, @activity, @frame)
  end
end
