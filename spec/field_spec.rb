
require 'rubygems'
require 'gosu'
require 'field'

class TestBox < Field::Box

  def initialize(x1, y1, x2, y2, l)
    set_position(x1,y1,x2,y2)
    @layer = l
  end

  attr_reader :layer

end

describe Field::Box do

  it "should preserve coordinates" do
    @b = TestBox.new(30, 0, 40, 10, 1)
    @b.x1.should == 30
    @b.x2.should == 40
    @b.y1.should == 0
    @b.y2.should == 10
  end

  it "should correct ordering of x coordinates if necessary" do
    @b = TestBox.new(40, 10, 30, 0, 1)
    @b.x1.should == 30
    @b.x2.should == 40
    @b.y1.should == 0
    @b.y2.should == 10
  end

end

describe Field, "hit testing" do

  before do
    @f = Field.new
  end

  it "should hit an odd layer box unless that box is completed eclipsed by even layer boxes" do
    
    # On a small grid, we can just color the boxes and test whether we actually intersect.
    150.times do
      f = Field.new
      b = Array.new(5) { Array.new(5,-1) }

      x1, x2 = rand(5), rand(5)
      x2, x2 = x2, x1 if x1 > x2
      y1, y2 = rand(5), rand(5)
      y2, y2 = y2, y1 if y1 > y2

      x1.should <= x2
      y1.should <= y2

      x1.upto(x2) do |x|
        y1.upto(y2) do |y|
          b[y][x] = 1
        end
      end

      ob = TestBox.new(x1, y1, x2, y2, 1)
      f.add(ob)

      x1, x2 = rand(5), rand(5)
      x2, x2 = x2, x1 if x1 > x2
      y1, y2 = rand(5), rand(5)
      y2, y2 = y2, y1 if y1 > y2

      x1.should <= x2
      y1.should <= y2

      x1.upto(x2) do |x|
        y1.upto(y2) do |y|
          b[y][x] = 2
        end
      end

      eb = TestBox.new(x1, y1, x2, y2, 2)
      f.add(eb)

      bf_hit_value = (b[3][3] == 1)
      f.hit?(3,3).should == bf_hit_value
    end
  end

end

describe Field, "load" do
  before do
    @f = Field.load(ImageLoader.new(nil))
  end
  def boxes
    @f.instance_variable_get("@boxes")
  end
  def backgrounds
    boxes.select{|b| b.class == Field::Background}
  end

  it "should contain all the backgrounds" do
    backgrounds.size.should == 5

    found = Array.new(5,false)
    backgrounds.each do |bg|
      bg.layer.should == 1
      found[bg.index] = true
    end

    found.select{|f| !f}.size.should == 0
  end

  it "should position the backgrounds in adjacent vertical stripes" do
    right_hand_side = 0
    updated = true 
    while updated
      updated = false 
      backgrounds.each do |bg|
        if bg.x1 == right_hand_side
          right_hand_side = bg.x2 + 1
          updated = true
        end
      end
    end

    right_hand_side.should == 11*640
  end

end
