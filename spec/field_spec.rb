
require 'rubygems'
require 'gosu'
require 'field'

describe Field::Box do

  it "should preserve coordinates" do
    @b = Field::Box.new(1, 30, 0, 40, 10, true)
    @b.x1.should == 30
    @b.x2.should == 40
    @b.y1.should == 0
    @b.y2.should == 10
  end

  it "should correct ordering of x coordinates if necessary" do
    @b = Field::Box.new(1, 40, 10, 30, 0, true)
    @b.x1.should == 30
    @b.x2.should == 40
    @b.y1.should == 0
    @b.y2.should == 10
  end

  it "should preserve layer" do
    @b = Field::Box.new(42, 40, 10, 30, 0, true)
    @b.layer.should == 42
  end

  it "should preserve solidity" do
    @b = Field::Box.new(42, 40, 10, 30, 0, true)
    @b.solid?.should == true
    @b.hollow?.should == false

    @b = Field::Box.new(42, 40, 10, 30, 0, false)
    @b.solid?.should == false
    @b.hollow?.should == true
  end
  
end

describe Field, "hit testing" do

  before do
    @f = Field.new
  end

  def query
    result = @f.boxes_intersecting(10,10,20,20)
    result.each { |b| b.class.should == Field::Box }
    result
  end

  describe "when empty" do
    it "should find no boxes intersecting" do
      query.should == []
    end
  end

  it "should find a box which contains query" do
    b = Field::Box.new(10, 0, 0, 40, 40, true)
    @f.add(b)
    query.should == [b]
  end

  it "should find a box which partially intersects" do
    b = Field::Box.new(10, 5, 5, 15, 15, true)
    @f.add(b)
    query.should == [b]
  end

  it "should not find a box which does not intersect" do
    b = Field::Box.new(10, 5, 5, 9, 15, true)
    @f.add(b)
    query.should == []
  end

  it "should pass brute force, random case test" do
    found_times = 0
    not_found_times = 0
    150.times do
      @f = Field.new
      x1 = rand(20)
      x2 = rand(20)
      y1 = rand(20)
      y2 = rand(20)
      @b = Field::Box.new(42, x1, y1, x2, y2, true)
      @f.add(@b)

      found = @f.boxes_intersecting(5,7,10,12).size > 0

      bf_found = false
      0.upto(19) do |x|
        0.upto(19) do |y|
          bf_found = true if x >= @b.x1 && x <= @b.x2 && y >= @b.y1 && y <= @b.y2 &&
                             x >= 5 && x <= 10 && y >= 7 && y <= 12
        end
      end

      found.should == bf_found
      if found
        found_times += 1
      else
        not_found_times += 1
      end
    end

    found_times.should >= 5
    not_found_times.should >= 5
  end

  it "should return all boxes in order by layer" do
    10.times do
      @f = Field.new
      10.times do 
        @f.add(Field::Box.new(rand(10), 0, 0, 40, 40, true))
      end
      l = -1
      @f.boxes_intersecting(10,10,20,20).each do |b|
        b.layer.should >= l
        l = b.layer
      end
    end
  end

end

