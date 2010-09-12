
require 'rubygems'
require 'gosu'
require 'game'

describe Game do

  before do
    @g = Game.new(ImageLoader.new(nil), nil)
  end

  it "should have a guy" do
    @g.guy.class.should == Guy
  end

  it "should have a field" do
    @g.field.class.should == Field
  end

  it "should have a viewport" do
    @g.viewport.class.should == Viewport
  end

  it "should initialize the viewport position to the guy's position" do
    @g.viewport.x1.should == @g.guy.x - 320
    @g.viewport.y1.should == @g.guy.y - 240
  end

end
