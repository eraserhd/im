
class ImageLoader
  def initialize(window)
    @cache = {}
    @window = window
  end

  def get(fname)
    if @window && !@cache[fname]
      @cache[fname] = Gosu::Image.new(@window, "data/#{fname}.png", false)
    end
    @cache[fname]
  end
end
