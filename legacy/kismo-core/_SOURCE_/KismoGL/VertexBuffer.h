/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoGL\VertexBuffer.h
 * Status: unique (no other copy of this file name in the archive)
 */


struct VertexAttribute
{
	Data::Vertices::eAttribute id;
	GLint size; //, Number fo gl elements (i.e. 3 for a float3)
	GLenum type;//, GL data type (i.e GL_FLOAT)
	GLsizei offset; //< offset into vertex buffer for this attribute
			
	VertexAttribute( Data::Vertices::eAttribute id = Data::Vertices::_Attribute_Count, GLint size = 0, GLenum type = 0, GLsizei offset = 0)
		: id(id) , size(size) , type(type) , offset(offset)
	{}

	typedef Vector<VertexAttribute,true> Array;
};

struct VertexBuffer
{
	UInt32 glBuffer; //, GL buffer handle
	VertexAttribute::Array attributes; //, The vertex attributes in thsi buffer
	GLsizei stride; //, Element stride in bytes between vertex elemnts

	ByteArray byteData; //< Byte data store for dynamic data

	UInt32 geometryUpdateMask; //, Mask from sceneNode::eGeometryUpdated used to filter whether the buffer needs updating
			
	VertexBuffer();
	~VertexBuffer();

	bool create();
	void destroy();

	typedef Vector<VertexBuffer> Array;
};