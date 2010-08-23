
require 'rubygems'
require 'gosu'
require 'guy'

describe Guy do
  
  after do
    @g.frame.should < @g.activity.frame_count
    @g.facing.should == Guy::RIGHT unless @g.facing == Guy::LEFT

    @g.facing.is_a?(Guy::Facing).should == true
    @g.activity.is_a?(Guy::Activity).should == true
  end

  FACINGS = Guy.constants.map{|c| Guy.const_get(c)}.select{|c| c.is_a?(Guy::Facing)}
  ACTIVITIES = Guy.constants.map{|c| Guy.const_get(c)}.select{|c| c.is_a?(Guy::Activity)}

  describe "when first created" do
    it "should initially be standing, facing left, at frame 0" do
      @g = Guy.new(nil)
      @g.activity.should == Guy::STANDING
      @g.facing.should == Guy::LEFT
      @g.frame.should == 0
    end
  end

  describe "when standing" do
    before do
      @g = Guy.new(nil)
    end
    it "should transition to running left when left key pressed" do
      @g.tick :left_pressed => true,
              :right_pressed => false,
              :space_pressed => false
      @g.facing.should == Guy::LEFT 
      @g.activity.should == Guy::RUNNING
    end

    it "should transition to running right when right key pressed" do
      @g.tick :left_pressed => false,
              :right_pressed => true,
              :space_pressed => false
      @g.facing.should == Guy::RIGHT
      @g.activity.should == Guy::RUNNING
    end

    it "should transition to flipping when space pressed" do
      @g.tick :left_pressed => false,
              :right_pressed => false,
              :space_pressed => true
      @g.facing.should == Guy::LEFT
      @g.activity.should == Guy::FLIPPING
      @g.frame.should == 0
    end
  end

  describe "when running" do

    before do
      @g = Guy.new(nil)
      tick
      @g.frame.should == 0
      @g.activity.should == Guy::RUNNING
      @g.facing.should == Guy::LEFT
    end
    def tick
      @g.tick :left_pressed => true,
              :right_pressed => false,
              :space_pressed => false
    end

    it "should indicate he is running" do
      @g.activity.should == Guy::RUNNING
      @g.facing.should == Guy::LEFT
    end

    it "should increment the frame each tick" do
      @g.frame.should == 0
      tick
      @g.frame.should == 1
      5.times do
        tick
      end
      @g.frame.should == 6
    end

    it "should wrap to frame zero at end of sequence" do
      13.times { tick }
      @g.frame.should == 13
      tick
      @g.frame.should == 0
    end

    describe "and the direction keys are released" do

      it "should return to standing" do
        tick
        @g.tick :left_pressed => false,
                :right_pressed => false,
                :space_pressed => false
        @g.activity.should == Guy::STANDING
      end

      it "should preserve facing direction" do
        @g.tick :right_pressed => true,
                :left_pressed => false
        @g.facing.should == Guy::RIGHT
        @g.tick :right_pressed => false,
                :left_pressed => false
        @g.facing.should == Guy::RIGHT
      end

      it "should reset the frame to zero when transitioning to standing" do
        @g.tick :right_pressed => false,
                :left_pressed => false
      end

    end

  end

  describe "when flipping" do

    before do
      @g = Guy.new(nil)
      @g.tick :left_pressed => false,
              :right_pressed => false,
              :space_pressed => true
      @g.facing.should == Guy::LEFT
      @g.activity.should == Guy::FLIPPING
      @g.frame.should == 0
    end

    it "should continue flipping when all keys released" do
      @g.tick :left_pressed => false,
              :right_pressed => false,
              :space_pressed => false
      @g.activity.should == Guy::FLIPPING
      @g.frame.should == 1
    end

    it "should flip again if space pressed when landing" do
      false.should == true
    end

    it "should transition to standing at end of flip when no keys pressed" do
      false.should == true
    end

    it "should transition to running at end of flip if direction key pressed" do
      false.should == true
    end

  end

end

describe Guy::ImageSet do

  before do
    $test_window = Gosu::Window.new(10,10,false) unless $test_window
    @image_set = Guy::ImageSet.new($test_window)
  end

  it "should raise an error when an invalid image is requested" do
    lambda {@image_set.get(Guy::LEFT, Guy::STANDING, 47)}.should raise_error(Guy::ImageSet::NoImage)
  end

  it "should not raise for all valid images" do
    FACINGS.each do |f|
      ACTIVITIES.each do |a|
        0.upto(a.frame_count - 1).each do |n|
          @image_set.get(f, a, n)
        end
      end
    end
  end

end

