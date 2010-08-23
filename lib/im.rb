
require 'rubygems'
require 'gosu'
require 'guy'

class GameWindow < Gosu::Window
  def initialize
    super(640, 480, false, 60)
    self.caption = "Unlikely Task"

    @background_image = Gosu::Image.new(self, "data/elevator-background-0.png", true)
    @guy = Guy.new(Guy::ImageSet.new(self))
  end

  def update
    @guy.tick :left_pressed => button_down?(Gosu::KbLeft),
              :right_pressed => button_down?(Gosu::KbRight),
              :fire_pressed => button_down?(Gosu::KbSpace)
  end

  def button_down(id)
    close if Gosu::KbQ == id
  end

  def draw
    @background_image.draw(0,0,0)
    @guy.image.draw(10,10,1)
  end
end

GameWindow.new.show
