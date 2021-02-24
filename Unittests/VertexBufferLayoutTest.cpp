#include "pch.h"
#include "globjects/VertexBufferLayout.h"
#include "globjects/LayoutElementType.h"
#include "application/Application.h"
#include "application/Window.h"
#include "glad/glad.h"

class LayoutElementTest : public ::testing::Test, protected otg::LayoutElementType {

public:
	LayoutElementTest():
		window("VertexBufferLayoutTest")
	{

	}

	~LayoutElementTest() {

	}

protected:
	otg::Application app;
	otg::Window window;
};

/*
* These tests are performed in one test fixure each because a window has to be created.
* (otherwise many windows would need to be created, which is verty annoying)
*/
TEST_F(LayoutElementTest, elementSize) {
	ASSERT_EQ(getElementTypeSize(otg::ElementType::Int), 4);
	ASSERT_EQ(getElementTypeSize(otg::ElementType::Uint), 4);
	ASSERT_EQ(getElementTypeSize(otg::ElementType::Short), 2);
	ASSERT_EQ(getElementTypeSize(otg::ElementType::Byte), 1);
	ASSERT_EQ(getElementTypeSize(otg::ElementType::Bool), 1);
	ASSERT_EQ(getElementTypeSize(otg::ElementType::Float), 4);
	ASSERT_EQ(getElementTypeSize(otg::ElementType::Double), 8);
}

TEST_F(LayoutElementTest, glTypes) {
	ASSERT_EQ(getElementGlType(otg::ElementType::Int), GL_INT);
	ASSERT_EQ(getElementGlType(otg::ElementType::Uint), GL_UNSIGNED_INT);
	ASSERT_EQ(getElementGlType(otg::ElementType::Short), GL_SHORT);
	ASSERT_EQ(getElementGlType(otg::ElementType::Byte), GL_BYTE);
	ASSERT_EQ(getElementGlType(otg::ElementType::Bool), GL_BOOL);
	ASSERT_EQ(getElementGlType(otg::ElementType::Float), GL_FLOAT);
	ASSERT_EQ(getElementGlType(otg::ElementType::Double), GL_DOUBLE);
}