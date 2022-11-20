#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "ModuleSceneIntro.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	int Pinball[148] = {
	-5, -10,
	394, -8,
	411, 624,
	231, 621,
	231, 597,
	329, 524,
	337, 517,
	341, 512,
	343, 504,
	345, 493,
	345, 460,
	344, 413,
	336, 403,
	329, 399,
	318, 392,
	318, 374,
	320, 382,
	325, 388,
	332, 390,
	339, 389,
	343, 384,
	344, 363,
	336, 352,
	349, 344,
	349, 558,
	351, 563,
	358, 567,
	365, 567,
	369, 562,
	373, 557,
	371, 124,
	367, 107,
	361, 92,
	355, 80,
	341, 67,
	329, 56,
	315, 48,
	296, 41,
	265, 34,
	241, 32,
	160, 31,
	128, 40,
	98, 53,
	81, 63,
	64, 78,
	49, 95,
	36, 114,
	27, 134,
	23, 144,
	19, 162,
	16, 178,
	15, 186,
	15, 295,
	19, 301,
	25, 303,
	30, 303,
	36, 301,
	38, 293,
	46, 291,
	15, 349,
	14, 354,
	14, 360,
	29, 376,
	17, 390,
	17, 501,
	18, 506,
	21, 514,
	24, 518,
	29, 524,
	40, 531,
	129, 593,
	130, 619,
	-4, 623,
	-4, -6
	};

	App->physics->CreateChain(0, 0, Pinball, 148);

	// Pivot 0, 0
	int Pinball2[100] = {
		235, 60,
		238, 56,
		245, 55,
		257, 55,
		269, 56,
		279, 59,
		288, 61,
		295, 68,
		296, 93,
		300, 98,
		306, 101,
		313, 99,
		322, 88,
		327, 87,
		332, 88,
		337, 95,
		343, 104,
		346, 113,
		348, 121,
		350, 130,
		351, 317,
		343, 323,
		341, 316,
		337, 310,
		330, 306,
		325, 304,
		325, 298,
		328, 291,
		331, 280,
		333, 270,
		335, 259,
		337, 249,
		338, 238,
		339, 226,
		339, 214,
		339, 203,
		338, 191,
		336, 180,
		333, 167,
		329, 155,
		325, 143,
		321, 134,
		315, 124,
		306, 112,
		296, 101,
		289, 93,
		278, 84,
		264, 74,
		252, 67,
		239, 61
	};

	App->physics->CreateChain(0, 0, Pinball2, 100);

	int Pinball3[92] = {
	226, 78,
	243, 87,
	256, 95,
	267, 104,
	279, 114,
	288, 124,
	296, 136,
	303, 149,
	309, 163,
	311, 173,
	312, 182,
	313, 194,
	313, 209,
	309, 227,
	306, 238,
	302, 248,
	297, 260,
	291, 271,
	284, 280,
	276, 284,
	270, 282,
	267, 276,
	268, 268,
	271, 262,
	277, 257,
	282, 251,
	289, 242,
	294, 233,
	298, 223,
	301, 213,
	302, 202,
	303, 194,
	303, 182,
	301, 170,
	298, 158,
	294, 149,
	288, 138,
	281, 127,
	274, 120,
	263, 110,
	252, 103,
	241, 98,
	234, 97,
	229, 95,
	227, 90,
	226, 81
	};

	App->physics->CreateChain(0, 0, Pinball3, 92);

	// Pivot 0, 0
	int Pinball4[112] = {
		84, 223,
		82, 208,
		82, 188,
		83, 175,
		85, 160,
		88, 146,
		91, 135,
		96, 125,
		102, 113,
		111, 100,
		117, 93,
		128, 85,
		138, 77,
		151, 73,
		157, 74,
		156, 108,
		143, 118,
		136, 126,
		131, 135,
		128, 146,
		127, 157,
		126, 165,
		128, 176,
		131, 186,
		135, 196,
		138, 203,
		137, 208,
		130, 211,
		123, 207,
		120, 198,
		117, 190,
		117, 179,
		117, 171,
		118, 163,
		122, 151,
		126, 141,
		132, 131,
		138, 122,
		139, 115,
		136, 108,
		131, 106,
		124, 107,
		119, 112,
		112, 119,
		106, 125,
		102, 131,
		98, 140,
		94, 147,
		91, 155,
		89, 163,
		87, 170,
		85, 180,
		84, 190,
		84, 201,
		84, 210,
		84, 218
	};

	App->physics->CreateChain(0, 0, Pinball4, 112);

	int Pinball5[22] = {
	45, 192,
	45, 204,
	45, 208,
	47, 210,
	49, 210,
	51, 208,
	52, 203,
	52, 192,
	51, 188,
	48, 187,
	46, 189
	};

	App->physics->CreateChain(0, 0, Pinball5, 22);

	int Pinball6[26] = {
	54, 497,
	122, 546,
	115, 555,
	47, 503,
	43, 498,
	40, 492,
	39, 487,
	39, 434,
	42, 429,
	47, 429,
	52, 432,
	57, 436,
	54, 494
	};

	App->physics->CreateChain(0, 0, Pinball6, 26);

	int Pinball7[26] = {
	304, 437,
	307, 497,
	239, 547,
	245, 555,
	315, 502,
	318, 498,
	320, 493,
	322, 487,
	322, 434,
	319, 429,
	314, 429,
	309, 432,
	306, 434
	};

	App->physics->CreateChain(0, 0, Pinball7, 26);

	/*b2Body* big_ball = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	big_ball->CreateFixture(&fixture)*/;

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property


	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points

	// TODO 4: If the player releases the mouse button, destroy the joint

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}


b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, float angle, bool collideConnected, bool enableLimit)
{
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = A->body;
	revoluteJointDef.bodyB = B->body;
	revoluteJointDef.collideConnected = collideConnected;
	revoluteJointDef.localAnchorA.Set(anchorA.x, anchorA.y);
	revoluteJointDef.localAnchorB.Set(anchorB.x, anchorB.y);
	revoluteJointDef.referenceAngle = 0;
	revoluteJointDef.enableLimit = enableLimit;
	revoluteJointDef.lowerAngle = -DEG_TO_RAD(angle);
	revoluteJointDef.upperAngle = DEG_TO_RAD(angle);

	return (b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef);
}