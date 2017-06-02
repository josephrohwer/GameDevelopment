#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_

#include <boost/shared_ptr.hpp>
#include <map>
#include "Rectangle.h"

class Sprite;
class Graphics;
class Map;

class Player {
private:
	enum MotionType {
		FIRST_MOTION_TYPE,
		STANDING = FIRST_MOTION_TYPE,
		WALKING,
		JUMPING,
		FALLING,
		LAST_MOTION_TYPE
	};

	enum HorizontalFacing {
		FIRST_HORIZONTAL_FACING,
		LEFT = FIRST_HORIZONTAL_FACING,
		RIGHT,
		LAST_HORIZONTAL_FACING
	};

	enum VerticalFacing {
		FIRST_VERTICAL_FACING,
		UP = FIRST_VERTICAL_FACING,
		DOWN,
		HORIZONTAL,
		LAST_VERTICAL_FACING
	};

	struct SpriteState {
		SpriteState(MotionType motion_type = STANDING,
			HorizontalFacing horizontal_facing = LEFT,
			VerticalFacing vertical_facing = HORIZONTAL) :
			motion_type(motion_type), 
			horizontal_facing(horizontal_facing), vertical_facing(vertical_facing) {}
		
		MotionType motion_type;
		HorizontalFacing horizontal_facing;
		VerticalFacing vertical_facing;
	};

	friend bool operator<(const SpriteState& a, const SpriteState& b);

	struct Jump {
	public:
		Jump() : time_remaining_ms_(0), active_(false) {}
		void update(int elapsed_time_ms);
		void reset();
		void reactivate() { active_ = time_remaining_ms_ > 0; }
		void deactivate() { active_ = false; }
		bool active() const { return active_;  }
	private:
		int time_remaining_ms_;
		bool active_;
	};

	void initializeSprites(Graphics& graphics);
	void initializeSprite(Graphics& graphics, const SpriteState& sprite);
	SpriteState getSpriteState();

	Rectangle leftCollision(int delta) const;
	Rectangle rightCollision(int delta) const;
	Rectangle topCollision(int delta) const;
	Rectangle bottomCollision(int delta) const;

	void updateX(int elapsed_time_ms, const Map& map);
	void updateY(int elapsed_time_ms, const Map& map);

	bool on_ground() const { return on_ground_; }
	std::map<SpriteState, boost::shared_ptr<Sprite>> sprites_;
	int x_;
	int y_;
	float velocity_x_;
	float velocity_y_;
	float acceleration_x_;
	HorizontalFacing horizontal_facing_;
	VerticalFacing vertical_facing_;
	bool on_ground_;
	Jump jump_;
public:
	Player(Graphics& graphics, int x, int y);
	void update(int elapsed_time_ms, const Map& map);
	void draw(Graphics& graphics);

	void startMovingLeft();
	void startMovingRight();
	void stopMoving();

	void lookUp();
	void lookDown();
	void lookHorizontal();

	void startJump();
	void stopJump();
};

#endif // !PLAYER_H_
