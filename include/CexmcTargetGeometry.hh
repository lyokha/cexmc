/*
 * =============================================================================
 *
 *       Filename:  CexmcTargetGeometry.hh
 *
 *    Description:  target geometry (static methods)
 *
 *        Version:  1.0
 *        Created:  03.12.2009 17:25:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_TARGET_GEOMETRY_HH
#define CEXMC_TARGET_GEOMETRY_HH

class  G4AffineTransform;


class  CexmcTargetGeometry
{
    public:
        static void  GetTargetTransform( G4AffineTransform &  transform );

    private:
        CexmcTargetGeometry();
};


#endif

