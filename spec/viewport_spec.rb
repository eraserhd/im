
require 'rubygems'
require 'gosu'
require 'viewport'

class MockGuy
  attr_reader :x, :y
  def initialize(x,y)
    @x = x
    @y = y
  end
end

describe Viewport do

  before do
    @v = Viewport.new(10+0, 15+0, 640*11, 480*5)
  end

  it "should start at position specified in constructor" do
    @v.x1.should == 10
    @v.y1.should == 15
    @v.width.should == 640
    @v.height.should == 480
    @v.field_width.should == 640*11
    @v.field_height.should == 480*5
  end

  it "should compute x2 and y2 correctly" do
    @v.x2.should == @v.x1 + @v.width - 1
    @v.y2.should == @v.y1 + @v.height - 1
  end

  describe "adjust_to_guy" do
    it "should not let the guy into viewport gutter" do
      10.times do
        new_x = rand(10*640+150)
        new_y = rand(4*480+150)
        guy = MockGuy.new(new_x,new_y)
        @v.adjust_to_guy(guy)
        @v.contains?(guy).should == true
      end
    end

    it "should not leave field" do
      @v.adjust_to_guy(MockGuy.new(25,25))
      @v.x1.should == 0
      @v.y1.should == 0

      @v.adjust_to_guy(MockGuy.new(@v.field_width-25, @v.field_height-25))
      @v.x1.should == @v.field_width-@v.width
      @v.y1.should == @v.field_height-@v.height
    end

  end
end
