/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoGL\Context_Win32_Members.h
 * Status: unique (no other copy of this file name in the archive)
 */

private:

		
	bool initialiseRenderDeviceContext();

	
	/** Creates a GL rendering surface with a pixel format as specified by its index as supplied by findRenderDevicePixelFormatID()		
	*/
	bool createRenderDeviceContext( int pixelFormat );

	/** Creates a new Context of the sepcified type using the default context created from createDefaultRenderDeviceContext() to access the functionality
		\remarks Eny existing textures/buffers etc on the context will be lost as a seperate/new context will be created
	*/
	bool setGLRenderContextVersion( int major = 3, int minor = 2 );

	/** Finds a pixel format index for the specified
		\remarks Requires an existing windows/context to query
	*/
	int findRenderDevicePixelFormatID( Count colourBits ); 
	
	void destroyWindow();