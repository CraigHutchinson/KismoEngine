/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiChartParser.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_CHARTPARSER_H
#define LEXAI_CHARTPARSER_H

#include "LexaiParser.h"
#include "GraphNode.h"

namespace lexai
{
	/** \brief Chart parser for optimal efficiency

	###################################
	Not implemented as it only deals with context free grammar ..or maybe it does then
	- If I get time this will be implemented as it should be much faster
	###################################

	*/
	//class ChartParser : public Parser
	//{
	//public:
	//	ChartParser( Grammar& grammar ) : Parser( grammar ) {}

	//protected:

	//	virtual Bool parse( Word::Array& words )
	//	{
	//		m_chart.resize( words.size() );
	//	}

	//	void addEdge( 

	//private:
	//	typedef Vector< List< Word > > Chart;
	//	Char m_chart; ////< The chart being processed
	//};

} //END: lexai

#endif