
require 'rubygems'
require 'gosu'
require 'game'

class GameWindow < Gosu::Window
  def initialize
    super(640, 480, false, 60)
    self.caption = "Unlikely Task"

    @game = Game.new(ImageLoader.new(self), Guy::ImageSet.new(self))
  end

  def update
    @game.tick :left_pressed => button_down?(Gosu::KbLeft),
               :right_pressed => button_down?(Gosu::KbRight),
               :space_pressed => button_down?(Gosu::KbSpace)
  end

  def button_down(id)
    close if Gosu::KbQ == id
  end

  def draw
    @game.draw
  end
end

GameWindow.new.show
