#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);

	Cube chassis2(info.chassis2_size.x, info.chassis2_size.y, info.chassis2_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis2.transform);

	Cube chassis3(info.chassis3_size.x, info.chassis3_size.y, info.chassis3_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis3.transform);

	Sphere head(info.head_size.x);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&head.transform);

	Cube body(info.body_size.x, info.body_size.y, info.body_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&body.transform);

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	btVector3 offset_2(info.chassis2_offset.x, info.chassis2_offset.y, info.chassis2_offset.z);
	offset_2 = offset_2.rotate(q.getAxis(), q.getAngle());

	btVector3 offset_3(info.chassis3_offset.x, info.chassis3_offset.y, info.chassis3_offset.z);
	offset_3 = offset_3.rotate(q.getAxis(), q.getAngle());

	btVector3 offset_head(info.head_offset.x, info.head_offset.y, info.head_offset.z);
	offset_head = offset_head.rotate(q.getAxis(), q.getAngle());

	btVector3 offset_body(info.body_offset.x, info.body_offset.y, info.body_offset.z);
	offset_body = offset_body.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	chassis2.transform.M[12] += offset_2.getX();
	chassis2.transform.M[13] += offset_2.getY();
	chassis2.transform.M[14] += offset_2.getZ();

	chassis3.transform.M[12] += offset_3.getX();
	chassis3.transform.M[13] += offset_3.getY();
	chassis3.transform.M[14] += offset_3.getZ();


	head.transform.M[12] += offset_head.getX();
	head.transform.M[13] += offset_head.getY();
	head.transform.M[14] += offset_head.getZ();


	body.transform.M[12] += offset_body.getX();
	body.transform.M[13] += offset_body.getY();
	body.transform.M[14] += offset_body.getZ();

	chassis.Render();
	chassis2.Render();
	chassis3.Render();
	head.Render();
	body.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

vec3 PhysVehicle3D::GetBackVector() const
{
	vec3 back_vector;
	btVector3 forward_vector = vehicle->getForwardVector();
	back_vector.Set(-(forward_vector.getX()), -(forward_vector.getY()), -(forward_vector.getZ()) );
	return back_vector;
}
