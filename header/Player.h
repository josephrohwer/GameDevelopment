#ifndef PLAYER_H_
#define PLAYER_H_

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <map>
#include "NumberSprite.h"
#include "Rectangle.h"
#include "Audio.h"
#include "Units.h"
#include "Timer.h"
#include "Sprite.h"
#include "VaryingWidthSprite.h"

class Graphics;
class Map;

class Player
{
public:
	Player(Graphics& graphics, units::Game x, units::Game y);

	void update(units::MS elapsed_time, const Map& map);
	void draw(Graphics& graphics);
	void drawHUD(Graphics& graphics);

	void startMovingLeft();
	void startMovingRight();
	void stopMoving();

	void lookUp();
	void lookDown();
	void lookHorizontal();

	void startJump(Mix_Chunk* jump_sound);
	void stopJump();

	void takeDamageFromBat(Mix_Chunk* damage_sound);
	void takeDamageFromLava(Mix_Chunk* damage_sound);

	Rectangle damageRectangle() const;

	units::Game center_x() const { return x_ + units::kHalfTile; }
private:
	enum MotionType
	{
		FIRST_MOTION_TYPE,
		STANDING = FIRST_MOTION_TYPE,
		INTERACTING,
		WALKING,
		JUMPING,
		FALLING,
		LAST_MOTION_TYPE
	};

	enum HorizontalFacing
	{
		FIRST_HORIZONTAL_FACING,
		LEFT = FIRST_HORIZONTAL_FACING,
		RIGHT,
		LAST_HORIZONTAL_FACING
	};

	enum VerticalFacing
	{
		FIRST_VERTICAL_FACING,
		UP = FIRST_VERTICAL_FACING,
		DOWN,
		HORIZONTAL,
		LAST_VERTICAL_FACING
	};

	struct SpriteState
	{
		SpriteState(MotionType motion_type = STANDING,
					HorizontalFacing horizontal_facing = LEFT,
					VerticalFacing vertical_facing = HORIZONTAL) :
					motion_type(motion_type), 
					horizontal_facing(horizontal_facing), 
					vertical_facing(vertical_facing) {}
		
		MotionType motion_type;
		HorizontalFacing horizontal_facing;
		VerticalFacing vertical_facing;
	};

	friend bool operator<(const SpriteState& a, const SpriteState& b);

	struct Health 
	{
	public:
		Health(Graphics& graphics);
		void update(units::MS elapsed_time);
		void draw(Graphics& graphics);
		bool takeDamage(units::HP damage);
	private:
		units::Game fillOffset(units::HP health) const;
		
		units::HP damage_;
		Timer damage_timer_;
		units::HP max_health_;
		units::HP current_health_;
		Sprite health_bar_sprite_;
		VaryingWidthSprite health_fill_sprite_;
		VaryingWidthSprite damage_fill_sprite_;
	};

	void initializeSprites(Graphics& graphics);
	void initializeSprite(Graphics& graphics, const SpriteState& sprite);
	SpriteState getSpriteState();

	Rectangle leftCollision(units::Game delta) const;
	Rectangle rightCollision(units::Game delta) const;
	Rectangle topCollision(units::Game delta) const;
	Rectangle bottomCollision(units::Game delta) const;

	void updateX(units::MS elapsed_time_ms, const Map& map);
	void updateY(units::MS elapsed_time_ms, const Map& map);

	bool spriteIsVisible() const;

	bool on_ground() const { return on_ground_; }

	std::map<SpriteState, boost::shared_ptr<Sprite>> sprites_;

	units::Game x_;
	units::Game y_;
	units::Velocity velocity_x_;
	units::Velocity velocity_y_;
	units::Acceleration acceleration_x_;
	
	HorizontalFacing horizontal_facing_;
	VerticalFacing vertical_facing_;
	bool on_ground_;
	bool jump_active_;
	bool interacting_;

	Health health_;
	Timer invincible_timer_;
};

#endif // !PLAYER_H_
