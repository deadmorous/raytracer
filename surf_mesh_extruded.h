// m_surf_mesh_extruded.h

#ifndef _SURF_MESH_EXTRUDED_H_AB0B81B0_CF3E_424F_9766_BA04D388199F_
#define _SURF_MESH_EXTRUDED_H_AB0B81B0_CF3E_424F_9766_BA04D388199F_

/// \file
/// \brief Utilities for the generation of surface meshes by extrusion.

#include "surf_mesh_common.h"

namespace surf_mesh {

struct ExtrudedMeshPart
    {
        int startIndex;
        vec_vertex vertices;
        Primitives primitives;
        XsLoops tubeEnd;
        explicit ExtrudedMeshPart( int startIndex = 0 ) : startIndex(startIndex) {}
        void addVertices( const vec_vertex& newVertices ) {
            int n = vertices.size();
            vertices.resize( n + newVertices.size() );
            std::copy( newVertices.begin(), newVertices.end(), vertices.begin() + n );
            }
    };

typedef std::list< ExtrudedMeshPart > ExtrudedMeshParts;



class ScaleVertex : public std::unary_function<vertex, vertex>
    {
    public:
        explicit ScaleVertex( float factor, const v2f& center = mkv2( 0.0f, 0.0f ) ) :
            m_factor(factor), m_center(center) {}
        vertex operator()( const vertex& v ) const {
            vertex result = v;
            for( int i=0; i<2; ++i )
                result[i] = m_center[i] + ( v[i] - m_center[i] )*m_factor;
            return result;
            }
    private:
        float m_factor;
        v2f m_center;
    };



class ExtrudeVertexConst : public std::binary_function<vertex, float, vertex>
    {
    public:
        vertex operator()( const vertex& v, float z ) const {
            vertex result = v;
            result[2] = z;
            return result;
            }
    };

class ExtrudeVertexLinear : public std::binary_function<vertex, float, vertex>
    {
    public:
        explicit ExtrudeVertexLinear( float cx = 0, float cy = 0, float z0 = 0 ) :
            m_z0(z0), m_cx(cx), m_cy(cy) {}
        vertex operator()( const vertex& v, float z ) const {
            vertex result = v;
            result[2] = z + m_z0 + m_cx*v[0] + m_cy*v[1];
            return result;
            }
        float cx() const {
            return m_cx;
            }
        float cy() const {
            return m_cy;
            }
        float z0() const {
            return m_z0;
            }
    private:
        float m_z0, m_cx, m_cy;
    };

class ExtrudeVertexQuad : public std::binary_function<vertex, float, vertex>
    {
    public:
        explicit ExtrudeVertexQuad(
                float cxx = 0,   float cyy = 0,   float cxy = 0,
                float cx = 0, float cy = 0, float z0 = 0 ) :
            m_cxx(cxx), m_cyy(cyy), m_cxy(cxy),
            m_z0(z0), m_cx(cx), m_cy(cy) {}
        vertex operator()( const vertex& v, float z ) const {
            vertex result = v;
            float x = v[0], y = v[1], xx = x*x, yy = y*y, xy = x*y;
            result[2] = z + m_z0 + m_cx*x + m_cy*y + m_cxx*xx + m_cyy*yy + 2*m_cxy*xy;
            return result;
            }
        float cx() const { return m_cx; }
        float cy() const { return m_cy; }
        float z0() const { return m_z0; }
        float cxx() const { return m_cxx; }
        float cyy() const { return m_cyy; }
        float cxy() const { return m_cxy; }
    private:
        float m_cxx, m_cyy, m_cxy;
        float m_z0, m_cx, m_cy;
    };



template< class ExtrudeVertex >
class NormalVectorCalculator : public std::unary_function<vertex, vertex> {};

template<>
class NormalVectorCalculator< ExtrudeVertexConst >
    {
    public:
        explicit NormalVectorCalculator( const ExtrudeVertexConst&, bool open ) : m_nz( open? -1.0f: 1.0f ) {}
        vertex operator()( const vertex& v ) const {
            vertex result = v;
            normal(result) = mkv3( 0.0f, 0.0f, m_nz );
            return result;
            }
    private:
        float m_nz;
    };

template<>
class NormalVectorCalculator< ExtrudeVertexLinear >
    {
    public:
        explicit NormalVectorCalculator( const ExtrudeVertexLinear& xv, bool open )
            {
            m_normal = mkv3( -xv.cx(), -xv.cy(), 1.0f );
            if( open )
                m_normal = -m_normal;
            }
        vertex operator()( const vertex& v ) const {
            vertex result = v;
            normal(result) = m_normal;
            return result;
            }
    private:
        v3f m_normal;
    };

template<>
class NormalVectorCalculator< ExtrudeVertexQuad >
    {
    public:
        explicit NormalVectorCalculator( const ExtrudeVertexQuad& xv, bool open ) : m_xv(xv), m_sgn(open? -1.0f: 1.0f) {}
        vertex operator()( const vertex& v ) const {
            vertex result = v;
            float x = v[0], y = v[1];
            float dxz = m_xv.cx() + 2*( m_xv.cxx()*x + m_xv.cxy()*y );
            float dyz = m_xv.cy() + 2*( m_xv.cyy()*y + m_xv.cxy()*x );
            normal(result) = m_sgn * mkv3( -dxz, -dyz, 1.0f );
            return result;
            }
    private:
        ExtrudeVertexQuad m_xv;
        float m_sgn;
    };

template< class ExtrudeVertex >
NormalVectorCalculator< ExtrudeVertex > makeNormalVectorCalculator( const ExtrudeVertex& xv, bool open ) {
    return NormalVectorCalculator< ExtrudeVertex >( xv, open );
    }



template< class ExtrudeVertex >
class ExtrudedVertexZ : public std::binary_function< vertex, float, float >
    {
    public:
        explicit ExtrudedVertexZ( const ExtrudeVertex& xv ) : m_xv(xv) {}
        float operator()(const vertex& v, float z) const {
            return m_xv(v, z)[2];
            }
    private:
        ExtrudeVertex m_xv;
    };

template< class ExtrudeVertex >
inline ExtrudedVertexZ< ExtrudeVertex > extrudedVertexZ( const ExtrudeVertex& xv ) {
    return ExtrudedVertexZ< ExtrudeVertex >( xv );
    }



template< class ExtrudeVertex >
inline float computeDeflection( const ExtrudeVertex& xv, const vec_vertex& vertices, const v2i& indexRange, bool inside );

template<>
inline float computeDeflection( const ExtrudeVertexConst&, const vec_vertex&, const v2i&, bool) {
    return 0;
    }

template<>
inline float computeDeflection( const ExtrudeVertexLinear&, const vec_vertex&, const v2i&, bool) {
    return 0;
    }

template<>
inline float computeDeflection( const ExtrudeVertexQuad& xv, const vec_vertex& vertices, const v2i& indexRange, bool inside )
    {
    vertex center = computeCenter( vertices, indexRange );
    v2f r0 = pos2( center );
    M2x2 b;
    b(0, 0) = xv.cxx();
    b(1, 1) = xv.cyy();
    b(1, 0) = b(0, 1) = xv.cxy();
    v2f c0 = b * r0;
    float z0 = r0.T() * c0;
    c0 *= -2.0f;
    ExtrudeVertexQuad xvd( xv.cxx(), xv.cyy(), xv.cxy(),   c0[0], c0[1],   z0 );
    typedef AccumRange<float> Acc;
    typedef ExtrudedVertexZ< ExtrudeVertexQuad > ExtZ;
    typedef std::binder2nd< ExtZ > ExtZ2;
    Composition< Acc, ExtZ2 > f( Acc(), ExtZ2( ExtZ( xvd ), 0.0f ) );
    forAllVertices( vertices, indexRange, f );
    if( inside )
        f( center );
    return f.f2.rangeSize();
    }



template< class ExtrudeVertex >
inline v2f capGenExtents( const Xs& xs, const ExtrudeVertex& xv )
    {
    typedef AccumRange<float> Acc;
    typedef ExtrudedVertexZ< ExtrudeVertex > ExtZ;
    typedef std::binder2nd< ExtZ > ExtZ2;
    Composition< Acc, ExtZ2 > f( Acc(), ExtZ2( ExtZ( xv ), 0.0f ) );
    forAllVertices( xs.vertices, f );
    return f.f2.range;
    }

template<>
inline v2f capGenExtents( const Xs&, const ExtrudeVertexConst& ) {
    return mkv2( 0.0f, 0.0f );
    }



template< class ExtrudeVertex >
class CapGenBase
    {
    public:
        CapGenBase() {}
        explicit CapGenBase( const ExtrudeVertex& xv ) : m_xv(xv) {}
        vec_vertex unproject( const vec_vertex& vertices, float z ) const {
            return transformVertices( vertices, std::bind2nd(m_xv, z) );
            }
        v2f extents( const Xs& xs ) const {
            return capGenExtents( xs, m_xv );
            }
        const ExtrudeVertex& xv() const {
            return m_xv;
            }
    private:
        ExtrudeVertex m_xv;
    };

template< class ExtrudeVertex >
class OpenCapGen : public CapGenBase< ExtrudeVertex >
    {
    public:
        typedef CapGenBase< ExtrudeVertex > BaseClass;

        OpenCapGen() {}
        explicit OpenCapGen( const ExtrudeVertex& xv ) : BaseClass(xv) {}
        ExtrudedMeshPart open( const Xs& xs, int /* levelOfDetail */, float z, int startIndex ) const {
            ExtrudedMeshPart result( startIndex );
            result.tubeEnd = incIndices( xs.loops, startIndex );
            return result;
            }
        ExtrudedMeshPart close( const Xs& xs, int /* levelOfDetail */, float z, int startIndex ) const {
            return ExtrudedMeshPart( startIndex );
            }
    };

typedef OpenCapGen< ExtrudeVertexConst > ConstOpenCapGen;

template< class ExtrudeVertex >
inline OpenCapGen< ExtrudeVertex > makeOpenCapGen( const ExtrudeVertex& xv ) {
    return OpenCapGen< ExtrudeVertex >( xv );
    }



template< class ExtrudeVertex >
inline ExtrudedMeshPart closeDoubleConnectedXs( const Xs& xs, float z, int startIndex, bool open, const ExtrudeVertex& xv, int lod )
    {
    // Extrude cross-section vertices
    ExtrudedMeshPart result( startIndex );

    Q_ASSERT( xs.loops.size() == 2 );
    const XsLoop *c1 = &xs.loops.front();
    const XsLoop *c2 = &xs.loops.back();
    int loopSize = c1->size();
    if( c2->size() != loopSize )
        throw cxx::exception( "surf_mesh::closeDoubleConnectedXs() failed: a little bit too complex cross-section - TODO" );
    if( c1->direction == CwXsLoop ) {
        Q_ASSERT( c2->direction == CcwXsLoop );
        std::swap( c1, c2 );
        }
    else {
        Q_ASSERT( c2->direction == CwXsLoop );
        }
    // At this point, c1 is expected to be the outer loop, and c1 - the inner loop

    // Compute number of slices to generate;
    // also compute the total number of extra vertices
    int n = xs.vertices.size();
    int sliceCount, nExtraVertices;
    if( lod > 0 ) {
        float deflection = computeDeflection( xv, xs.vertices, mkv2i( 0, n ), false );
        float diameter = c1->box( xs.vertices ).maxSize();
        float relThickness = 1 - c2->box( xs.vertices ).maxSize() / diameter;
        const float DeflectionSensitivity = 0.35f;
        float sliceParam = std::min( DeflectionSensitivity*lod*deflection*relThickness/diameter, 1.0f );
        const int MaxSlices = 5;
        sliceCount = static_cast<int>( linterp( 1, MaxSlices, sliceParam, 0.0f, 1.0f ) );
        nExtraVertices = ( sliceCount - 1 ) * loopSize;
        }
    else {
        sliceCount = 1;
        nExtraVertices = 0;
        }

    // Compute vertices
    // Allocate array of resulting vertices
    result.vertices.resize( n + nExtraVertices );

    // Extrude cross-section vertices first
    std::binder2nd< ExtrudeVertex > xvz = std::bind2nd( xv, z );
    vec_vertex::iterator itResVertex = result.vertices.begin();
    writeVertices( transformVertices( xs.vertices, xvz ), itResVertex );

    // Generate additional vertices
    for( int slice=1; slice<sliceCount; ++slice ) {
        float sliceParam = static_cast<float>( slice ) / sliceCount;
        vec_vertex vbuf( loopSize );
        std::transform(
            xs.vertices.begin() + c1->indexRange[0], xs.vertices.begin() + c1->indexRange[1],
            xs.vertices.begin() + c2->indexRange[0],
            vbuf.begin(), LinterpFunc<vertex>( sliceParam ) );
        transformVerticesInPlace( vbuf, xvz );
        writeVertices( vbuf, itResVertex );
        }
    Q_ASSERT( itResVertex == result.vertices.end() );

    // Generate primitives
    for( int slice=0; slice<sliceCount; ++slice ) {
        int iouter = slice == 0 ?   c1->indexRange[0] :   n + loopSize*( slice - 1 );
        int iinner = slice == sliceCount - 1 ?   c2->indexRange[0] :   n + loopSize*slice;
        result.primitives.push_back( Primitive::triangleStrip(
            startIndex + iouter,
            startIndex + iinner,
            loopSize, !open ) );
        }

    // Generate normal vectors
    transformVerticesInPlace( result.vertices, makeNormalVectorCalculator( xv, open ) );

    // Set tube end
    result.tubeEnd = incIndices( xs.loops, startIndex + nExtraVertices );

    return result;
    }

template< class ExtrudeVertex >
inline ExtrudedMeshPart closeSingleConnectedXs( const Xs& xs, float z, int startIndex, bool open, const ExtrudeVertex& xv, int lod )
    {
    ExtrudedMeshPart result( startIndex );

    // Compute number of slices to generate for each loop;
    // also compute the total number of extra vertices
    vec_int sliceCounts( xs.loops.size() );
    int nExtraVertices = 0;
    int iloop = 0;
    if( lod > 0 ) {
        for( XsLoops::const_iterator it=xs.loops.begin(); it!=xs.loops.end(); ++it, ++iloop ) {
            const XsLoop& loop = *it;
            float deflection = computeDeflection( xv, xs.vertices, loop.indexRange, true );
            float diameter = loop.box( xs.vertices ).maxSize();
            const float DeflectionSensitivity = 0.35f;
            float sliceParam = std::min( DeflectionSensitivity*lod*deflection/diameter, 1.0f );
            const int MaxSlices = 5;
            int sliceCount = sliceCounts[iloop] = static_cast<int>( linterp( 1, MaxSlices, sliceParam, 0.0f, 1.0f ) );
            nExtraVertices += ( sliceCount - 1 ) * loop.size() + 1;
            }
        }
    else {
        std::fill( sliceCounts.begin(), sliceCounts.end(), 1 );
        nExtraVertices = xs.loops.size();
        }

    // Allocate array of resulting vertices
    int n = xs.vertices.size();
    result.vertices.resize( n + nExtraVertices );

    // Extrude cross-section vertices first
    std::binder2nd< ExtrudeVertex > xvz = std::bind2nd( xv, z );
    vec_vertex::iterator itResVertex = result.vertices.begin();
    writeVertices( transformVertices( xs.vertices, xvz ), itResVertex );

    // Generate additional vertices and primitives
    iloop = 0;
    for( XsLoops::const_iterator it=xs.loops.begin(); it!=xs.loops.end(); ++it, ++iloop ) {
        const XsLoop& loop = *it;

        // Compute position of the center of the loop
        vertex center = loop.center( xs.vertices );

        // Get the index of first extra vertex in this loop
        int ixv = itResVertex - result.vertices.begin();

        // Generate slice vertices
        int sliceCount = sliceCounts[iloop];
        for( int slice=1; slice<sliceCount; ++slice ) {
            float sliceScale = static_cast<float>( sliceCount - slice ) / sliceCount;
            writeVertices(
                transformVertices(
                    xs.vertices, loop.indexRange,
                    composition( xvz, ScaleVertex( sliceScale, pos2(center) ) ) ),
                itResVertex );
            }

        // Generate vertex at the center of the loop
        *itResVertex++ = xvz( center );

        // Generate triangle strips for all slices except the innermost one
        int loopSize = loop.size();
        for( int slice=1; slice<sliceCount; ++slice ) {
            int iouter = slice == 1 ?   loop.indexRange[0] :   ixv + ( slice - 2 )*loopSize;
            int iinner = ixv + ( slice - 1 )*loopSize;
            result.primitives.push_back( Primitive::triangleStrip(
                startIndex + iouter,
                startIndex + iinner,
                loopSize, !open ) );
            }

        // Generate triangle fan primitive for the innermost slice
        result.primitives.push_back( Primitive::triangleFan(
            startIndex + ixv+(sliceCount-1)*loopSize,
            startIndex + (sliceCount > 1 ?   ixv+(sliceCount-2)*loopSize :   loop.indexRange[0]),
            loopSize, !open ) );
        }
    Q_ASSERT( itResVertex == result.vertices.end() );

    // Generate normal vectors
    transformVerticesInPlace( result.vertices, makeNormalVectorCalculator( xv, open ) );

    // Set tube end
    result.tubeEnd = incIndices( xs.loops, startIndex + nExtraVertices );
    return result;
    }

template< class ExtrudeVertex >
inline ExtrudedMeshPart closeXs( const Xs& xs, float z, int startIndex, bool open, const ExtrudeVertex& xv, int lod )
    {
    int ncw = 0,   nccw = 0;
    for( XsLoops::const_iterator it=xs.loops.begin(); it!=xs.loops.end(); ++it )
        ++( it->direction == CwXsLoop? ncw: nccw );
    if( ncw == 1   &&   nccw == 1 )
        return closeDoubleConnectedXs( xs, z, startIndex, open, xv, lod );
    else if( ncw == 0   ||   nccw == 0 )
        return closeSingleConnectedXs( xs, z, startIndex, open, xv, lod );
    else if( ncw == 0   &&   nccw == 0 )
        return ExtrudedMeshPart();
    else
        throw cxx::exception( "surf_mesh::closeXs() failed: too complex cross-section" );
    }

template< class ExtrudeVertex >
class ClosedCapGen : public CapGenBase< ExtrudeVertex >
    {
    public:
        typedef CapGenBase< ExtrudeVertex > BaseClass;

        ClosedCapGen() {}
        explicit ClosedCapGen( const ExtrudeVertex& xv ) : BaseClass(xv) {}
        ExtrudedMeshPart open( const Xs& xs, int levelOfDetail, float z, int startIndex ) const {
            ExtrudedMeshPart result = closeXs( xs, z, startIndex, true, this->xv(), levelOfDetail );
            result.tubeEnd = incIndices( xs.loops, startIndex + result.vertices.size() );
            return result;
            }
        ExtrudedMeshPart close( const Xs& xs, int levelOfDetail, float z, int startIndex ) const {
            ExtrudedMeshPart result = closeXs( xs, z, startIndex, false, this->xv(), levelOfDetail );
            return result;
            }
    };

typedef ClosedCapGen< ExtrudeVertexConst > ConstClosedCapGen;

template< class ExtrudeVertex >
inline ClosedCapGen< ExtrudeVertex > makeClosedCapGen( const ExtrudeVertex& xv ) {
    return ClosedCapGen< ExtrudeVertex >( xv );
    }



class SetAxialTextureCoordinate : public std::unary_function< vertex, vertex >
    {
    public:
        SetAxialTextureCoordinate( float z1, float z2 ) : m_z1(z1), m_z2(z2) {}
        vertex operator()( const vertex& v ) const {
            vertex result = v;
            if( m_z1 != m_z2 )
                tex(result)[1] = linterpNoClamp( 0.0f, 1.0f, v[2], m_z1, m_z2 );
            return result;
            }
    private:
        float m_z1, m_z2;
    };


class ExtrudedMeshBuilder
    {
    public:
        explicit ExtrudedMeshBuilder( float z = 0, int levelOfDetail = MediumLevelOfDetail ) :
            m_lod(levelOfDetail), m_z0(z), m_z(z), m_startIndex(0), m_isOpen(false) {}

        template< class XSG, class CG >
        void open( const XSG& xg, const CG& cg )
            {
            Q_ASSERT( !m_isOpen );

            // Generate cross-section and find its diameter
            m_xs = xg.crossSection( m_lod );
            m_d = xg.crossSectionDiameter( m_xs );
            Q_ASSERT( m_d > 0 );

            // Save opening cap extents
            m_openCapExt = cg.extents( m_xs );

            // Open cap
            addPart( cg.open( m_xs, m_lod, m_z, m_startIndex ) );
            ExtrudedMeshPart part;
            part.vertices = cg.unproject( m_xs.vertices, m_z );
            addPart( part );

            m_isOpen = true;
            }

        template< class CG >
        void close( float length, const CG& cg )
            {
            Q_ASSERT( m_isOpen );

            // Obtain closing cap extents
            v2f closeCapExt = cg.extents( m_xs );

            // Compute adjusted z positions that take inner cap extents into account
            float z1 = m_z + m_openCapExt[1];
            float z2 = m_z + length + closeCapExt[0];
            float L = z2 - z1;
            if( L <= 0 )
                throw cxx::exception( "surf_mesh::Builder::close() failed: too large cap inner extents" );

            // Compute the number of cross-sections in this part of the tube
            float shapeFactor = L/m_d;
            int MinCount = 2,   MaxCount = static_cast<int>( 5*shapeFactor + 0.5 );
            int count = linterp( MinCount, MaxCount, m_lod, MinLevelOfDetail, MaxLevelOfDetail );

            // Generate boundary surface
            // Note: Primitive indices in this mesh part will refer to vertices from next part
            ExtrudedMeshPart part;
            float dz = L/count,   z = z1 + dz;
            for( int i=0; i+1<count; ++i, z+=dz )
                part.addVertices( transformVertices( m_xs.vertices, std::bind2nd(ExtrudeVertexConst(), z) ) );
            addBoundaryIndices( part, m_xs.loops, count );
            addPart( part );

            // Increase current z coordinate
            m_z += length;

            // Add vertices at the edge of the boundary surface
            part = ExtrudedMeshPart();
            part.vertices = cg.unproject( m_xs.vertices, m_z );
            addPart( part );

            // Add closing part of the mesh
            addPart( cg.close( m_xs, m_lod, m_z, m_startIndex ) );

            m_isOpen = false;
            }

        void skip( float skipLength )
            {
            Q_ASSERT( !m_isOpen );
            m_z += skipLength;
            }

        template< class XSG, class CG >
        void closeAndOpen( float length, const XSG& xg, const CG& cg, float skipLength = 0 )
            {
            close( length, cg );
            skip( skipLength );
            open( xg, cg );
            }

        Mesh mesh() const
            {
            Q_ASSERT( !m_isOpen );

            Mesh result = Mesh::fromParts( m_parts );

            // Set axial texture coordinate such that it will change from 0 (at start) to 1 (at end)
            transformVerticesInPlace( result.vertices, SetAxialTextureCoordinate(m_z0, m_z) );

            return result;
            }

        float z0() const {
            return m_z0;
            }

        float z() const {
            return m_z;
            }

        float length() const {
            return m_z - m_z0;
            }

        bool isOpen() const {
            return m_isOpen;
            }

    private:
        int m_lod;
        float m_z0;
        float m_z;
        int m_startIndex;
        bool m_isOpen;

        ExtrudedMeshParts m_parts;
        Xs m_xs;
        float m_d;
        v2f m_openCapExt;

        void addPart( const ExtrudedMeshPart& part )
            {
            m_parts.push_back( part );
            m_startIndex += part.vertices.size();
            }

        void addBoundaryPartIndices( ExtrudedMeshPart& part, const XsLoop& loop, int count ) const
            {
            Primitive primitive( Primitive::TriangleStrip );
            int nx = m_xs.vertices.size();
            int nl = loop.size(),   _2nl = nl << 1;
            primitive.indices.resize( _2nl );
            vec_int::iterator itprim = primitive.indices.begin();
            int i0 = m_startIndex + loop.indexRange[0];
            int d1 = loop.direction == CcwXsLoop? 0: -nx;
            int d2 = -d1 - nx;
            for( int i=0; i<nl; ++i, ++i0 ) {
                *itprim++ = i0 + d1;
                *itprim++ = i0 + d2;
                }
            Q_ASSERT( itprim == primitive.indices.end() );
            for( int i=0; i<count; ++i ) {
                part.primitives.push_back( primitive );
                incIndicesInPlace( primitive.indices, nx );
                }
            }

        void addBoundaryIndices( ExtrudedMeshPart& part, const XsLoops& loops, int count ) const
            {
            for( XsLoops::const_iterator it=loops.begin(); it!=loops.end(); ++it )
                addBoundaryPartIndices( part, *it, count );
            }
    };

} // end namespace surf_mesh

#endif // _SURF_MESH_EXTRUDED_H_AB0B81B0_CF3E_424F_9766_BA04D388199F_
