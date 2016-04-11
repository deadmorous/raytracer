// surf_mesh_revolved.h

#ifndef _SURF_MESH_REVOLVED_H_AB0B81B0_CF3E_424F_9766_BA04D388199F_
#define _SURF_MESH_REVOLVED_H_AB0B81B0_CF3E_424F_9766_BA04D388199F_

#include "surf_mesh_common.h"

namespace surf_mesh {

class VertexTransform : public std::unary_function< vertex, vertex >
    {
    public:
        typedef fsmx::MX< fsmx::Data<4,1,float> > v4f;
        typedef fsmx::MX< fsmx::Data<3,3,float> > M3x3;
        typedef fsmx::MX< fsmx::Data<4,4,float> > M4x4;

        VertexTransform() :
            m_t( identity<4>() ), m_n( identity<3>() ) {}

        VertexTransform& translate( const v3f& x )
            {
            translation(m_t) += affine(m_t) * x;
            return *this;
            }

        VertexTransform& rotate( const v3f& axis, float angle )
            {
            M3x3 P = rotation( axis, angle );
            affine(m_t) *= P;
            m_n *= P;
            return *this;
            }

        VertexTransform& scale( float factor )
            {
            affine(m_t) *= factor;
            return *this;
            }

        VertexTransform& set( const M4x4& t )
            {
            m_t = t;
            m_n = affine(t).inv().T();
            return *this;
            }

        vertex operator()( const vertex& v ) const {
            vertex result = v;
            v4f r;
            r.block<3,1>(0,0) = position(v);
            r[3] = 1.0f;
            r = m_t*r;
            position(result) = r.block<3,1>(0,0);
            normal(result) = m_n * normal(v);
            return result;
            }

    private:
        typedef fsmx::MX< fsmx::SliceProxy< 3, 3, 4, 1, float, float > > AffineProxy;
        typedef fsmx::MX< fsmx::SliceProxy< 3, 3, 4, 1, float, const float > > ConstAffineProxy;
        typedef fsmx::MX< fsmx::SliceProxy< 3, 1, 4, 1, float, float > > TranslationProxy;
        typedef fsmx::MX< fsmx::SliceProxy< 3, 1, 4, 1, float, const float > > ConstTranslationProxy;
        M4x4 m_t;   // Position transformation
        M3x3 m_n;   // Normal vector transformation

        template< int N >
        static fsmx::MX< fsmx::Data<N,N,float> > diag( float x ) {
            fsmx::MX< fsmx::Data<N,N,float> > result;
            result.fill( 0.0f );
            result.diag().fill( x );
            return result;
            }

        template< int N >
        static fsmx::MX< fsmx::Data<N,N,float> > identity() {
            return diag<N>( 1.0f );
            }

        static M3x3 spin( const v3f& unitAxis ) {
            M3x3 result;
            result.diag().fill( 0.0f );
            result( 1, 0 ) =  unitAxis[2];
            result( 0, 1 ) = -unitAxis[2];
            result( 2, 0 ) = -unitAxis[1];
            result( 0, 2 ) =  unitAxis[1];
            result( 2, 1 ) =  unitAxis[0];
            result( 1, 2 ) = -unitAxis[0];
            return result;
            }

        static M3x3 rotation( const v3f& axis, float angle )
            {
            if( angle == 0.0f )
                return identity<3>();
            v3f n = axis / static_cast<float>( sqrt( axis.T() * axis ) );
            float c = cos(angle),   s = sin(angle);
            return diag<3>(c) + (1-c)*( n*n.T() ) + s*spin( n );
            }

        static M4x4 fromAffine( const M3x3& t ) {
            M4x4 result;
            translation(result).fill( 0.0f );
            affine(result) = t;
            result.block<1,3>( 3, 0 ).fill( 0.0f );
            result( 3, 3 ) = 1.0f;
            return result;
            }

        static AffineProxy affine( M4x4& t ) {
            return t.block<3,3>( 0, 0 );
            }
        static ConstAffineProxy affine( const M4x4& t ) {
            return t.block<3,3>( 0, 0 );
            }
        static TranslationProxy translation( M4x4& t ) {
            return t.block<3,1>( 0, 3 );
            }
        static ConstTranslationProxy translation( const M4x4& t ) {
            return t.block<3,1>( 0, 3 );
            }
    };

class VertexRevolutionParam
    {
    public:
        const VertexTransform& initialTransform() const {
            return m_initialTransform;
            }

        VertexRevolutionParam& setInitialTransform( const VertexTransform& initialTransform )
            {
            m_initialTransform = initialTransform;
            return *this;
            }

        const VertexTransform& stepTransform() const {
            return m_stepTransform;
            }

        VertexRevolutionParam& setStepTransform( const VertexTransform& stepTransform )
            {
            m_stepTransform = stepTransform;
            return *this;
            }

    private:
        VertexTransform m_initialTransform;
        VertexTransform m_stepTransform;
    };

class RevolvedMeshBuilder
    {
    public:
        explicit RevolvedMeshBuilder( int levelOfDetail = MediumLevelOfDetail ) :
            m_lod(levelOfDetail), m_startIndex(0) {}

        // Determines how many revolution steps should be done for revolution by totalAngle
        int revolutionSteps( float angle ) const
            {
            float f = fabs(angle) / static_cast<float>(2*M_PI);
            float fMinCount = std::max( 3.0f*f, 1.0f ), fMaxCount = std::max( 75.0f*f, fMinCount );
            float fcount = linterp( fMinCount, fMaxCount, m_lod, MinLevelOfDetail, MaxLevelOfDetail );
            return static_cast<int>( fcount );
            }

        template< class XSG >
        void addPart( const XSG& xg, const VertexRevolutionParam& param, int stepCount )
            {
            // Generate cross-section
            Xs xs = xg.crossSection( m_lod );
            int n = xs.vertices.size();

            // Create new mesh part
            m_parts.push_back( RevolvedMeshPart( m_startIndex ) );
            RevolvedMeshPart& pt = m_parts.back();

            // Generate vertices
            int vertexCount = n * ( 1 + stepCount );
            pt.vertices.resize( vertexCount );
            vec_vertex::iterator it = pt.vertices.begin();
            std::transform( xs.vertices.begin(), xs.vertices.end(), it, param.initialTransform() );
            for( int i=0; i<stepCount; ++i, it+=n )
                std::transform( it, it+n, it+n, param.stepTransform() );

            // Generate vertex texture coordinates
            float tex = 0,   dtex = stepCount > 1 ?   1.0f / ( stepCount - 1 ) :   0;
            it = pt.vertices.begin();
            for( int i=0; i<=stepCount; ++i, tex+=dtex, it+=n )
                std::transform( it, it+n, it, SetRevolutionSurfaceTexureCoordinates(tex) );

            if( stepCount > 0 ) {
                // Generate primitives

                // Decide how to organize primitives better
                if( xs.loops.size() == 1   &&   n >= stepCount ) {
                    // Generate meridional strips
                    bool fwd = xs.loops.front().direction == CcwXsLoop;
                    for( int i=0, i1=0; i<stepCount; ++i, i1+=n )
                        pt.primitives.push_back(
                            Primitive::triangleStrip( m_startIndex+i1, m_startIndex+i1+n, n, fwd ) );
                    }
                else {
                    // Generate lateral strips
                    for( XsLoops::const_iterator it=xs.loops.begin(); it!=xs.loops.end(); ++it ) {
                        const XsLoop& loop = *it;
                        Q_ASSERT( loop.size() > 1 );
                        bool fwd = loop.direction == CcwXsLoop;
                        for( int il=loop.indexRange[0]; il+1<loop.indexRange[1]; ++il )
                            pt.primitives.push_back( Primitive::triangleStrip(
                                m_startIndex+il, m_startIndex+il+1, stepCount+1, fwd, n ) );
                        }
                    }
                }

            // Increase start index
            m_startIndex += vertexCount;
            }

        Mesh mesh() const {
            return Mesh::fromParts( m_parts );
            }

    private:
        int m_lod;
        int m_startIndex;

        struct RevolvedMeshPart
            {
                int startIndex;
                vec_vertex vertices;
                Primitives primitives;
                explicit RevolvedMeshPart( int startIndex = 0 ) : startIndex(startIndex) {}
            };

        typedef std::list< RevolvedMeshPart > RevolvedMeshParts;
        RevolvedMeshParts m_parts;

        class SetRevolutionSurfaceTexureCoordinates : public std::unary_function< vertex, vertex >
            {
            public:
                SetRevolutionSurfaceTexureCoordinates( float meridian ) : m_meridian(meridian) {}
                vertex operator()( const vertex& v ) const
                    {
                    vertex result = v;
                    tex(result) = mkv2( m_meridian, tex(v)[0] );
                    return result;
                    }
            private:
                float m_meridian;
            };
    };

} // end namespace surf_mesh

#endif // _SURF_MESH_REVOLVED_H_AB0B81B0_CF3E_424F_9766_BA04D388199F_
