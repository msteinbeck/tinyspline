require 'tinysplineruby'

module TinySpline
  class BSpline < Tinysplineruby::BSpline
    def ctrlp
        super.dup
    end
    
    def ctrlp=(ctrlp)
      setCtrlp(ctrlp)
    end

    def knots
        super.dup
    end

    def knots=(knots)
      setKnots(knots)
    end
  end

  class DeBoorNet < Tinysplineruby::DeBoorNet
    def points
        super.dup
    end

    def result
        super.dup
    end
  end

  class Utils < Tinysplineruby::Utils
  end

  TS_OPENED = Tinysplineruby::TS_OPENED
  TS_NONE = Tinysplineruby::TS_NONE
  TS_CLAMPED = Tinysplineruby::TS_CLAMPED
  TS_BEZIERS = Tinysplineruby::TS_BEZIERS
end
