%module tinysplinedlang

%dmethodmodifiers tinyspline::BSpline::toString "override public";
%dmethodmodifiers tinyspline::ChordLengths::toString "override public";
%dmethodmodifiers tinyspline::DeBoorNet::toString "override public";
%dmethodmodifiers tinyspline::Domain::toString "override public";
%dmethodmodifiers tinyspline::Frame::toString "override public";
%dmethodmodifiers tinyspline::FrameSeq::toString "override public";
%dmethodmodifiers tinyspline::Morphism::toString "override public";
%dmethodmodifiers tinyspline::Vec2::toString "override public";
%dmethodmodifiers tinyspline::Vec3::toString "override public";
%dmethodmodifiers tinyspline::Vec4::toString "override public";

%include "tinyspline.i"

namespace std {
	%template(Vector) vector<tinyspline::real>;
};
