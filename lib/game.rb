
require 'guy'
require 'field'
require 'viewport'
require 'image_loader'

class Game

  def initialize(image_loader, guy_images)
    @guy = Guy.new(guy_images)
    @field = Field.load(image_loader)
    @viewport = Viewport.new(@guy.x-320, @guy.y-240, 11*640, 5*480)
    @viewport_x = @guy.x - 320
    @viewport_y = @guy.y - 240
  end

  def tick(options)
    @guy.tick(options)
    @viewport.adjust_to_guy(@guy)
  end

  def draw
    @field.draw(@viewport)
    @guy.draw(@viewport)
  end

  attr_reader :field, :guy, :viewport
end
