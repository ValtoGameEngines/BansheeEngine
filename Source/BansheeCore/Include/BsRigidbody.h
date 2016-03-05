//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsCorePrerequisites.h"
#include "BsPhysicsCommon.h"

namespace BansheeEngine
{
	/** @addtogroup Physics
	 *  @{
	 */

	/** Type of force or torque that can be applied to a rigidbody. */
	enum class ForceMode
	{
		Force, /**< Value applied is a force. */
		Impulse, /**< Value applied is an impulse (i.e. a direct change in its linear or angular momentum). */
		Velocity, /**< Value applied is velocity. */
		Acceleration /**< Value applied is accelearation. */
	};

	/** Type of force that can be applied to a rigidbody at an arbitrary point. */
	enum class PointForceMode
	{
		Force, /**< Value applied is a force. */
		Impulse, /**< Value applied is an impulse (i.e. a direct change in its linear or angular momentum). */
	};

	/**
	 * Rigidbody is a dynamic physics object that can be moved using forces (or directly). It will interact with other
	 * static and dynamic physics objects in the scene accordingly (i.e. it will push other non-kinematic rigidbodies, 
	 * and collide with static objects).
	 *
	 * The shape and mass of a rigidbody is governed by its colliders. You must attach at least one collider for the 
	 * rigidbody to be valid.
	 */
	class BS_CORE_EXPORT Rigidbody
	{
	public:
		/** Flags that control options of a Rigidbody object. */
		enum class Flag
		{
			/** No options. */
			None = 0x00,
			/** Automatically calculate center of mass transform and inertia tensors from child shapes (colliders). */
			AutoTensors = 0x01,
			/** Calculate mass distribution from child shapes (colliders). Only relevant when auto-tensors is on. */
			AutoMass = 0x02,
			/** 
			 * Enables continous collision detection. This can prevent fast moving bodies from tunneling through each other.
			 * This must also be enabled globally in Physics otherwise the flag will be ignored.
			 */
			CCD = 0x04
		};

		/** Determines interpolation mode for a rigidbody transform during physics simulation. */
		enum class InterpolationMode
		{
			/** 
			 * No interpolation is performed, physics transform is copied straight to the rigidbody when physics tick is 
			 * done. 
			 */
			None, 
			/** 
			 * Physics transfrom from the most recent tick is saved and slowly interpolated to during the following render 
			 * frames. This can improve smoothness of the visible movement at framerates higher than the physics simulation 
			 * but will introduce a delay of one physics tick to all such objects. This can create slight inconsistencies as
			 * non-interpolated objects will have no such delay, as well as cause input lag due to the delayed reaction.
			 */
			Interpolate, 
			/** 
			 * Physics transform movement will be extrapolated from the last physics simulation tick. This will improve
			 * smoothness of visible movement at framerates higher than the physics simulation. Unlike Interpolate it will
			 * not introduce an input delay, but will introduce an error as the exact position/rotation of the objects is
			 * extrapolated from the last frame's movement and velocities. 
			 */
			Extrapolate
		};

		/** 
		 * Constructs a new rigidbody. 
		 *
		 * @param[in]	linkedSO	Scene object that owns this rigidbody. All physics updates applied to this object
		 *							will be transfered to this scene object (i.e. the movement/rotation resulting from
		 *							those updates).
		 */
		Rigidbody(const HSceneObject& linkedSO);
		virtual ~Rigidbody();

		/** 
		 * Moves the rigidbody to a specific position. This method will ensure physically correct movement, i.e. the body
		 * will collide with other objects along the way.
		 */
		virtual void move(const Vector3& position) = 0;

		/** 
		 * Rotates the rigidbody. This method will ensure physically correct rotation, i.e. the body will collide with other
		 * objects along the way.
		 */
		virtual void rotate(const Quaternion& rotation) = 0;

		/** Returns the current position of the rigidbody. */
		virtual Vector3 getPosition() const = 0;

		/** Returns the current rotation of the rigidbody. */
		virtual Quaternion getRotation() const = 0;

		/** 
		 * Moves and rotates the rigidbody to a specific position. Unlike move() and rotate() this will not transform the
		 * body in a physically correct manner, but will instead "teleport" it immediately to the specified position and
		 * rotation.
		 */
		virtual void setTransform(const Vector3& pos, const Quaternion& rot) = 0;

		/** 
		 * Sets the mass of the object and all of its collider shapes. Only relevant if Flag::AutoMass or Flag::AutoTensors
		 * is turned off. Value of zero means the object is immovable (but can be rotated).
		 */
		virtual void setMass(float mass) = 0;

		/** 
		 * Returns the mass of the object. This may be mass manually set by setMass(), or the mass of all child colliders,
		 * depending if the mass is calculated automatically or not.
		 */
		virtual float getMass() const = 0;

		/** 
		 * Sets if the body is kinematic. Kinematic body will not move in response to external forces (e.g. gravity, or
		 * another object pushing it), essentially behaving like collider. Unlike a collider though, you can still move
		 * the object and have other dynamic objects respond correctly (i.e. it will push other objects).
		 */
		virtual void setIsKinematic(bool kinematic) = 0;

		/** Checks if the body is kinematic. */
		virtual bool getIsKinematic() const = 0;

		/** 
		 * Checks if the body is sleeping. Objects that aren't moved/rotated for a while are put to sleep to reduce load
		 * on the physics system. 
		 */
		virtual bool isSleeping() const = 0;

		/** Forces the object to sleep. Useful if you know the object will not move in any significant way for a while. */
		virtual void sleep() = 0;

		/** 
		 * Wakes an object up. Useful if you modified properties of this object, and potentially surrounding objects which
		 * might result in the object being moved by physics (although the physics system will automatically wake the
		 * object up for majority of such cases).
		 */
		virtual void wakeUp() = 0;

		/** Sets a threshold of force and torque under which the object will be considered to be put to sleep. */
		virtual void setSleepThreshold(float threshold) = 0;

		/** Gets a threshold of force and torque under which the object will be considered to be put to sleep. */
		virtual float getSleepThreshold() const = 0;

		/** Sets whether or not the rigidbody will have the global gravity force applied to it. */
		virtual void setUseGravity(bool gravity) = 0;

		/** Gets whether or not the rigidbody will have the global gravity force applied to it. */
		virtual bool getUseGravity() const = 0;

		/** Sets the linear velocity of the body. */
		virtual void setVelocity(const Vector3& velocity) = 0;

		/** Returns the current linear velocity of the body. */
		virtual Vector3 getVelocity() const = 0;

		/** Sets the angular velocity of the body. */
		virtual void setAngularVelocity(const Vector3& velocity) = 0;

		/** Returns the current angular velocity of the body. */
		virtual Vector3 getAngularVelocity() const = 0;

		/** Sets the linear drag of the body. Higher drag values means the object resists linear movement more. */
		virtual void setDrag(float drag) = 0;

		/** Gets the linear drag of the body. Higher drag values means the object resists linear movement more. */
		virtual float getDrag() const = 0;

		/** Sets the angular drag of the body. Higher drag values means the object resists angular movement more. */
		virtual void setAngularDrag(float drag) = 0;

		/** Gets the angular drag of the body. Higher drag values means the object resists angular movement more. */
		virtual float getAngularDrag() const = 0;

		/** 
		 * Sets the inertia tensor in local mass space. Inertia tensor determines how difficult is to rotate the object.
		 * Values of zero in the inertia tensor mean the object will be unable to rotate around a specific axis. Only 
		 * relevant if Flag::AutoTensors is turned off.
		 */
		virtual void setInertiaTensor(const Vector3& tensor) = 0;

		/** Gets the inertia tensor in local mass space.  */
		virtual Vector3 getInertiaTensor() const = 0;

		/** Returns the maximum angular velocity of the rigidbody. Velocity will be clamped to this value. */
		virtual void setMaxAngularVelocity(float maxVelocity) = 0;

		/** Gets the maximum angular velocity of the rigidbody. */
		virtual float getMaxAngularVelocity() const = 0;

		/**
		 * Sets the rigidbody's center of mass transform. Only relevant if Flag::AutoTensors is turned off.
		 *
		 * @param[in]	position	Position of the center of mass.
		 * @param[in]	rotation	Rotation that determines orientation of the inertia tensor (rotation of the center of 
		 *							mass frame).
		 */
		virtual void setCenterOfMass(const Vector3& position, const Quaternion& rotation) = 0;

		/** Returns the position of the center of mass. */
		virtual Vector3 getCenterOfMassPosition() const = 0;

		/** Returns the rotation of the inertia tensor. */
		virtual Quaternion getCenterOfMassRotation() const = 0;

		/** 
		 * Sets the number of iterations to use when solving for position. Higher values can improve precision and 
		 * numerical stability of the simulation. 
		 */
		virtual void setPositionSolverCount(UINT32 count) = 0;

		/** Gets the number of iterations to use when solving for position. */
		virtual UINT32 getPositionSolverCount() const = 0;


		/** 
		 * Sets the number of iterations to use when solving for velocity. Higher values can improve precision and 
		 * numerical stability of the simulation. 
		 */
		virtual void setVelocitySolverCount(UINT32 count) = 0;

		/** Gets the number of iterations to use when solving for velocity. */
		virtual UINT32 getVelocitySolverCount() const = 0;

		/** Sets interpolation mode that controls how is the rigidbody transfrom updated from the physics simulation. */
		virtual void setInterpolationMode(InterpolationMode value) { mInterpolationMode = value; }

		/** Gets interpolation mode that controls how is the rigidbody transfrom updated from the physics simulation. */
		virtual InterpolationMode getInterpolationMode() const { return mInterpolationMode; }

		/** Sets flags that control the behaviour of the rigidbody. */
		virtual void setFlags(Flag flags) { mFlags = flags; }

		/** Gets flags that control the behaviour of the rigidbody. */
		virtual Flag getFlags() const { return mFlags; }

		/** 
		 * Applies a force to the center of the mass of the rigidbody. This will produce linear momentum.
		 *
		 * @param[in]	force		Force to apply.
		 * @param[in]	mode		Determines what is the type of @p force.
		 */
		virtual void addForce(const Vector3& force, ForceMode mode = ForceMode::Force) = 0;

		/** 
		 * Applies a torque to the rigidbody. This will produce angular momentum.
		 *
		 * @param[in]	torque		Torque to apply.
		 * @param[in]	mode		Determines what is the type of @p torque.
		 */
		virtual void addTorque(const Vector3& torque, ForceMode mode = ForceMode::Force) = 0;

		/** 
		 * Applies a force to a specific point on the rigidbody. This will in most cases produce both linear and angular
		 * momentum.
		 *
		 * @param[in]	force		Force to apply.
		 * @param[in]	position	World position to apply the force at.
		 * @param[in]	mode		Determines what is the type of @p force.
		 */
		virtual void addForceAtPoint(const Vector3& force, const Vector3& position, 
			PointForceMode mode = PointForceMode::Force) = 0;

		/** 
		 * Returns the total (linear + angular) velocity at a specific point. 
		 *
		 * @param[in]	point	Point in world space.
		 * @return				Total velocity of the point.
		 */
		virtual Vector3 getVelocityAtPoint(const Vector3& point) const = 0;

		/** Registers a new collider as a child of this rigidbody. */
		virtual void addCollider(FCollider* collider) = 0;

		/** Removes a collider from the child list of this rigidbody. */
		virtual void removeCollider(FCollider* collider) = 0;

		/** Removes all colliders from the child list of this rigidbody. */
		virtual void removeColliders() = 0;

		/** 
		 * Recalculates rigidbody's mass, inertia tensors and center of mass depending on the currently set child colliders.
		 * This should be called whenever relevant child collider properties change (like mass or shape).
		 *
		 * If automatic tensor calculation is turned off then this will do nothing. If automatic mass calculation is turned
		 * off then this will use the mass set directly on the body using setMass().
		 */
		virtual void updateMassDistribution() { }

		/** 
		 * Creates a new rigidbody. 
		 *
		 * @param[in]	linkedSO	Scene object that owns this rigidbody. All physics updates applied to this object
		 *							will be transfered to this scene object (i.e. the movement/rotation resulting from
		 *							those updates).
		 */
		static SPtr<Rigidbody> create(const HSceneObject& linkedSO);

		/** Triggered when one of the colliders owned by the rigidbody starts colliding with another object. */
		Event<void(const CollisionData&)> onCollisionBegin;

		/** Triggered when a previously colliding collider stays in collision. Triggered once per frame. */
		Event<void(const CollisionData&)> onCollisionStay;

		/** Triggered when one of the colliders owned by the rigidbody stops colliding with another object. */
		Event<void(const CollisionData&)> onCollisionEnd;

		/** @cond INTERNAL */

		/** 
		 * Sets the priority of the physics update. Bodies with a higher priority will be updated before the bodies with
		 * lower priority. This allows you to control the order of updated in case rigidbodies are in some way dependant.
		 */
		void _setPriority(UINT32 priority);

		/** Sets a unique ID of the rigidbody, so it can be recognized by the physics system. */
		void _setPhysicsId(UINT32 id) { mPhysicsId = id; }

		/** 
		 * Applies new transform values retrieved from the most recent physics update (i.e. values resulting from physics 
		 * simulation). 
		 */
		void _setTransform(const Vector3& position, const Quaternion& rotation);

		/** 
		 * Sets the object that owns this physics object, if any. Used for high level systems so they can easily map their
		 * high level physics objects from the low level ones returned by various queries and events.
		 */
		void _setOwner(PhysicsOwnerType type, void* owner) { mOwner.type = type; mOwner.ownerData = owner; }

		/** 
		 * Gets the object that owns this physics object, if any. Used for high level systems so they can easily map their
		 * high level physics objects from the low level ones returned by various queries and events.
		 */
		void* _getOwner(PhysicsOwnerType type) const { return mOwner.type == type ? mOwner.ownerData : nullptr; }

		/** @endcond */

	protected:
		friend class FCollider;

		Flag mFlags = Flag::None;
		InterpolationMode mInterpolationMode = InterpolationMode::None;
		PhysicsObjectOwner mOwner;
		UINT32 mPriority = 0;
		UINT32 mPhysicsId = 0;
		HSceneObject mLinkedSO;
	};

	/** @} */
}