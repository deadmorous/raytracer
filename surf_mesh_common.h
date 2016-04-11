// m_surf_mesh_common.h

#ifndef _SURF_MESH_COMMON_H_AB0B81B0_CF3E_424F_9766_BA04D388199F_
#define _SURF_MESH_COMMON_H_AB0B81B0_CF3E_424F_9766_BA04D388199F_

#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include "fsmx.h"
#include "m_const.h"
#include "cxx_exception.h"
#include <cmath>

namespace surf_mesh {

template< class X  >
inline void ClampValue( X& x, X xmin, X xmax )
    {
    if( xmax < xmin )
        std::swap( xmax, xmin );
    if( x < xmin )
        x = xmin;
    else if( x > xmax )
        x = xmax;
    }

typedef fsmx::MX< fsmx::Data<8,1,float> > vertex;
typedef fsmx::MX< fsmx::Data<3,1,float> > v3f;
typedef fsmx::MX< fsmx::Data<2,1,float> > v2f;
typedef fsmx::MX< fsmx::Data<2,2,float> > M2x2;
typedef fsmx::MX< fsmx::Data<2,1,int> > v2i;

typedef fsmx::MX< fsmx::SliceProxy< 3, 1, vertex::Rs, vertex::Cs, float, float > > vertexProxy3;
typedef fsmx::MX< fsmx::SliceProxy< 2, 1, vertex::Rs, vertex::Cs, float, float > > vertexProxy2;
typedef fsmx::MX< fsmx::SliceProxy< 2, 1, v3f::Rs, v3f::Cs, float, float > > v3fProxy2;

template< class T >
inline fsmx::MX< fsmx::Data<2,1,T> > mkv2( T x1, T x2 ) {
    return fsmx::mxMake< fsmx::MX< fsmx::Data<2,1,T> > >( x1, x2 );
    }

inline v2i mkv2i( int x1, int x2 ) {
    return mkv2( x1, x2 );
    }

template< class T >
inline fsmx::MX< fsmx::Data<3,1,T> > mkv3( T x1, T x2, T x3 = 0 ) {
    return fsmx::mxMake< fsmx::MX< fsmx::Data<3,1,T> > >( x1, x2, x3 );
    }

inline vertex mkvertex( float x1=0, float x2=0, float x3=0, float n1=0, float n2=0, float n3=0, float t1=0, float t2=0 ) {
    return fsmx::mxMake< vertex >( x1, x2, x3, n1, n2, n3, t1, t2 );
    }

template< class V >
inline fsmx::MX< fsmx::SliceProxy< 3, 1, V::Rs, V::Cs, typename V::value_type, typename V::value_type > > position( V& v ) {
    return v.template block< 3, 1 >( 0, 0 );
    }

template< class V >
inline fsmx::MX< fsmx::SliceProxy< 3, 1, V::Rs, V::Cs, typename V::value_type, const typename V::value_type > > position( const V& v ) {
    return v.template block< 3, 1 >( 0, 0 );
    }

template< class V >
inline fsmx::MX< fsmx::SliceProxy< 2, 1, V::Rs, V::Cs, typename V::value_type, typename V::value_type > > pos2( V& v ) {
    return v.template block< 2, 1 >( 0, 0 );
    }

template< class V >
inline fsmx::MX< fsmx::SliceProxy< 2, 1, V::Rs, V::Cs, typename V::value_type, const typename V::value_type > > pos2( const V& v ) {
    return v.template block< 2, 1 >( 0, 0 );
    }

template< class V >
inline fsmx::MX< fsmx::SliceProxy< 3, 1, V::Rs, V::Cs, typename V::value_type, typename V::value_type > > normal( V& v ) {
    return v.template block< 3, 1 >( 3, 0 );
    }

template< class V >
inline fsmx::MX< fsmx::SliceProxy< 3, 1, V::Rs, V::Cs, typename V::value_type, const typename V::value_type > > normal( const V& v ) {
    return v.template block< 3, 1 >( 3, 0 );
    }

template< class V >
inline fsmx::MX< fsmx::SliceProxy< 2, 1, V::Rs, V::Cs, typename V::value_type, typename V::value_type > > normal2( V& v ) {
    return v.template block< 2, 1 >( 3, 0 );
    }

template< class V >
inline fsmx::MX< fsmx::SliceProxy< 2, 1, V::Rs, V::Cs, typename V::value_type, const typename V::value_type > > normal2( const V& v ) {
    return v.template block< 2, 1 >( 3, 0 );
    }

template< class V >
inline fsmx::MX< fsmx::SliceProxy< 2, 1, V::Rs, V::Cs, typename V::value_type, typename V::value_type > > tex( V& v ) {
    return v.template block< 2, 1 >( 6, 0 );
    }

template< class V >
inline fsmx::MX< fsmx::SliceProxy< 2, 1, V::Rs, V::Cs, typename V::value_type, const typename V::value_type > > tex( const V& v ) {
    return v.template block< 2, 1 >( 6, 0 );
    }

template< class X, class T >
struct ProductType {};

template<> struct ProductType<int, int> { typedef int product_type; };
template<> struct ProductType<int, float> { typedef float product_type; };
template<> struct ProductType<float, int> { typedef float product_type; };
template<> struct ProductType<float, float> { typedef float product_type; };
template<> struct ProductType<double, double> { typedef double product_type; };
template<> struct ProductType<double, int> { typedef double product_type; };
template<> struct ProductType<int, double> { typedef double product_type; };
template<> struct ProductType<float, double> { typedef double product_type; };
template<> struct ProductType<double, float> { typedef double product_type; };

template< class X, class T >
typename ProductType<X,T>::product_type linterp( X xmin, X xmax, T t, T tmin, T tmax )
    {
    ClampValue( t, tmin, tmax );
    return ( xmin * (tmax-t) + xmax * (t-tmin) ) / (tmax-tmin);
    }

template< class X, class T >
typename ProductType<X,T>::product_type linterpNoClamp( X xmin, X xmax, T t, T tmin, T tmax )
    {
    return ( xmin * (tmax-t) + xmax * (t-tmin) ) / (tmax-tmin);
    }

template< class T >
class LinterpFunc : public std::binary_function< T, T, T >
    {
    public:
        LinterpFunc( float param ) : m_param( param ) {}
        T operator()( const T& x1, const T& x2 ) const {
            return x1*( 1.0f - m_param ) + x2*m_param;
            }
    private:
        float m_param;
    };



static const int MinLevelOfDetail = 0;
static const int MediumLevelOfDetail = 5;
static const int MaxLevelOfDetail = 10;



typedef std::vector<vertex> vec_vertex;
typedef std::vector<int> vec_int;

// Note: Xs in class names stands for cross-section

template< class T >
inline v2i vectorIndexRange( const std::vector<T>& v ) {
    return mkv2i( 0, v.size() );
    }

template< class F >
inline void forAllVertices(const vec_vertex& vertices, const v2i& indexRange, const F& f)
    {
    for( int i=indexRange[0]; i<indexRange[1]; ++i )
        f(vertices[i]);
    }

template< class F >
inline void forAllVertices(const vec_vertex& vertices, const v2i& indexRange, F& f)
    {
    for( int i=indexRange[0]; i<indexRange[1]; ++i )
        f(vertices[i]);
    }

template< class F >
inline void forAllVertices(const vec_vertex& vertices, const F& f) {
    forAllVertices( vertices, vectorIndexRange(vertices), f );
    }

template< class F >
inline void forAllVertices(const vec_vertex& vertices, F& f) {
    forAllVertices( vertices, vectorIndexRange(vertices), f );
    }



inline void writeVertices( const vec_vertex& vertices, vec_vertex::iterator& it )
    {
    std::copy( vertices.begin(), vertices.end(), it );
    it += vertices.size();
    }



template< class F >
inline vec_vertex transformVertices( const vec_vertex& vertices, const v2i& indexRange, const F& f )
    {
    vec_vertex result( indexRange[1] - indexRange[0] );
    std::transform( vertices.begin() + indexRange[0], vertices.begin() + indexRange[1], result.begin(), f );
    return result;
    }

template< class F >
inline vec_vertex transformVertices( const vec_vertex& vertices, const F& f ) {
    return transformVertices( vertices, vectorIndexRange( vertices ), f );
    }

template< class F >
inline void transformVerticesInPlace( vec_vertex& vertices, const v2i& indexRange, const F& f ) {
    std::transform( vertices.begin() + indexRange[0], vertices.begin() + indexRange[1], vertices.begin() + indexRange[0], f );
    }

template< class F >
inline void transformVerticesInPlace( vec_vertex& vertices, const F& f ) {
    transformVerticesInPlace( vertices, vectorIndexRange( vertices ), f );
    }



// Copy-pasted & edited from std because std::binder1st didn't compile.
template<typename Op> class mybinder1st :
    public std::unary_function<typename Op::second_argument_type, typename Op::result_type>
    {
    private:
        Op op;
        typename Op::first_argument_type value;

    public:
        mybinder1st(const Op& op, const typename Op::first_argument_type& value) : op(op), value(value) {}
        typename Op::result_type operator()(const typename Op::second_argument_type& value2) const {
            return op(value, value2);
            }
    };

// Copy-pasted & edited from std because we now use mybinder1st rather than std::binder1st.
template<typename Op, typename V1>
inline mybinder1st<Op> mybind1st(const Op& op, const V1& v1) {
    return mybinder1st<Op>(op, v1);
    }

struct XsBox
    {
        bool empty;
        v2f xmin;
        v2f xmax;
        XsBox() : empty( true ) {}
        void accum( const vertex& x )
            {
            if( empty ) {
                xmin = xmax = pos2( x );
                empty = false;
                }
            else for( int i=0; i<2; ++i ) {
                if( xmin[i] > x[i] )
                    xmin[i] = x[i];
                else if( xmax[i] < x[i] )
                    xmax[i] = x[i];
                }
            }
        float maxSize() const {
            if( empty )
                return 0.0f;
            v2f d = xmax - xmin;
            return std::max( d[0], d[1] );
            }
    };

template< class T >
struct AccumRange
    {
        typedef fsmx::MX< fsmx::Data<2,1,T> > Range;
        typedef T argument_type;
        typedef void result_type;
        bool empty;
        Range range;
        AccumRange() : empty( true ) {}
        void accum( const T& x ) {
            if( empty ) {
                range = mkv2( x, x );
                empty = false;
                }
            else if( range[0] > x )
                range[0] = x;
            else if( range[1] < x )
                range[1] = x;
            }
        void operator()( const T& x ) {
            accum( x );
            }
        T rangeSize() const {
            if( empty )
                return 0;
            else
                return range[1] - range[0];
            }
    };

template< class F2, class F1 >
struct Composition
    {
        F2 f2;
        F1 f1;
        Composition( const F2& f2, const F1& f1 ) : f2(f2), f1(f1) {}
        typename F2::result_type operator()( typename F1::argument_type x ) {
            return f2( f1( x ) );
            }
    };

template< class F2, class F1 >
Composition<F2, F1> composition( const F2& f2, const F1& f1 ) {
    return Composition<F2, F1>( f2, f1 );
    }

enum XsLoopDirection
    {
    CcwXsLoop,
    CwXsLoop
    };

inline vertex computeCenter( const vec_vertex& vertices, const v2i& indexRange )
    {
    vertex result = mkvertex( 0, 0 );
    forAllVertices( vertices, indexRange, mybind1st( std::mem_fun(&vertex::operator+=<vertex::Data>), &result ) );
    Q_ASSERT( indexRange[1] - indexRange[0] > 0 );
    result *= 1.0f / ( indexRange[1] - indexRange[0] );
    return result;
    }

inline vertex computeCenter( const vec_vertex& vertices ) {
    return computeCenter( vertices, vectorIndexRange(vertices) );
    }

inline XsBox computeBox( const vec_vertex& vertices, const v2i& indexRange )
    {
    XsBox result;
    forAllVertices( vertices, indexRange, mybind1st( std::mem_fun(&XsBox::accum), &result ) );
    return result;
    }

inline XsBox computeBox( const vec_vertex& vertices ) {
    return computeBox( vertices, vectorIndexRange(vertices) );
    }

struct XsLoop
    {
        v2i indexRange;
        XsLoopDirection direction;
        XsLoop() : indexRange( mkv2(0, 0) ), direction(CcwXsLoop) {}
        XsLoop( const v2i& indexRange, XsLoopDirection direction ) : indexRange(indexRange), direction(direction) {}

        int size() const {
            return indexRange[1] - indexRange[0];
            }

        template< class F >
        void forAllVertices( const vec_vertex& vertices, const F& f ) {
            surf_mesh::forAllVertices( vertices, indexRange, f );
            }
        vec_vertex pickVertices( const vec_vertex& vertices ) const {
            vec_vertex result( size() );
            std::copy( vertices.begin()+indexRange[0], vertices.begin()+indexRange[1], result.begin() );
            return result;
            }

        vertex center(const vec_vertex& vertices) const {
            return computeCenter( vertices, indexRange );
            }
        XsBox box( const vec_vertex& vertices ) const {
            return computeBox( vertices, indexRange );
            }
    };

typedef std::list< XsLoop > XsLoops;

struct Xs
    {
        // Array of cross-section vertices ordered such that the outer contour
        // is traversed CCW, and inner contours are traversed CW; outer contour
        // should be the first, followed by other (if any).
        vec_vertex vertices;

        // The size of this array is the number of cross-section loops;
        // each element describes one componenet.
        XsLoops loops;

        template< class F >
        void forAllVertices( const F& f ) {
            surf_mesh::forAllVertices( vertices, f );
            }

        vertex center() const {
            return computeCenter( vertices );
            }
        XsBox box() const {
            return computeBox( vertices );
            }
        void setOneLoop( XsLoopDirection direction ) {
            loops.clear();
            loops.push_back( XsLoop( mkv2i( 0, vertices.size() ), direction ) );
            }
    };

struct Primitive
    {
        enum Type {
            TriangleStrip, TriangleFan, Triangles
            };
        Type type;
        vec_int indices;
        explicit Primitive( Type type = TriangleStrip ) : type(type) {}
        Primitive( Type type, const vec_int& indices ) : type(type), indices(indices) {}

        static Primitive triangleStrip( int i1, int i2, int size, bool forward = true, int stride = 1 ) {
            Primitive result( TriangleStrip );
            result.indices.resize( size << 1 );
            if( forward )
                std::swap( i1, i2 );
            for( int i=0, _2i=0, istr=0; i<size; ++i, _2i+=2, istr+=stride ) {
                result.indices[_2i  ] = i1 + istr;
                result.indices[_2i+1] = i2 + istr;
                }
            return result;
            }

        static Primitive triangleFan( int icenter, int ifan, int size, bool forward = true ) {
            Primitive result( TriangleFan );
            result.indices.resize( size + 1 );
            result.indices[0] = icenter;
            if( forward )
                for( int i=0; i<size; ++i )
                    result.indices[i+1] = ifan + i;
            else
                for( int i=0; i<size; ++i )
                    result.indices[i+1] = ifan + size - 1 - i;
            return result;
            }
    };

typedef std::list< Primitive > Primitives;



struct Mesh
    {
        vec_vertex vertices;
        Primitives primitives;

        template< class Parts >
        static Mesh fromParts( const Parts& parts )
            {
            typedef typename Parts::const_iterator PartIt;

            // Count vertices
            int vertexCount = 0;
            for( PartIt it=parts.begin(); it!=parts.end(); ++it )
                vertexCount += it->vertices.size();

            Mesh result;

            // Copy vertices and primitives to the mesh
            result.vertices.resize( vertexCount );
            vec_vertex::iterator itResVertex = result.vertices.begin();
            for( PartIt it=parts.begin(); it!=parts.end(); ++it ) {
                writeVertices( it->vertices, itResVertex );
                std::copy( it->primitives.begin(), it->primitives.end(), std::back_inserter( result.primitives ) );
                }
            return result;
            }
    };



inline void incIndicesInPlace( vec_int& indices, int delta ) {
    std::transform( indices.begin(), indices.end(), indices.begin(), std::bind2nd( std::plus<int>(), delta ) );
    }

inline v2i incIndices( const v2i& indexRange, int delta ) {
    return mkv2( indexRange[0]+delta, indexRange[1]+delta );
    }

inline XsLoop incIndices( const XsLoop& loop, int delta )
    {
    if( delta == 0 )
        return loop;
    else
        return XsLoop( incIndices( loop.indexRange, delta ), loop.direction );
    }

inline XsLoops incIndices( const XsLoops& loops, int delta )
    {
    if( delta == 0 )
        return loops;
    XsLoops result;
    for( XsLoops::const_iterator it=loops.begin(); it!=loops.end(); ++it )
        result.push_back( incIndices( *it, delta ) );
    return result;
    }

class CircularXsGen
    {
    public:
        explicit CircularXsGen( float radius, const v2f& pos=mkv2(0.0f,0.0f) ) :
            m_radius( radius ), m_pos(mkv3(pos[0], pos[1])) {
            Q_ASSERT( radius > 0 );
            }

        Xs crossSection( int levelOfDetail ) const
            {
            Xs result;

            const int MinCount = 3, MaxCount = 75;
            int count = linterp( MinCount, MaxCount, levelOfDetail, MinLevelOfDetail, MaxLevelOfDetail );
            float dphi = static_cast<float>( 2*M_PI / count );
            float c = cos(dphi),   s = sin(dphi);
            M2x2 P;
            P(0,0) = P(1,1) = c;
            P(1,0) = s;
            P(0,1) = -s;
            v3f v = mkv3( 1.0f, 0.0f );
            v3fProxy2 v2 = pos2( v );
            result.vertices.resize( count+1 );
            float t = 0,   dt = 1.0f / count;
            for( int i=0; i<=count; ++i, v2=P*v2, t+=dt ) {
                vertex r;
                position( r ) = v * m_radius + m_pos;
                normal( r ) = v;
                tex( r ) = mkv2( t, 0.0f );
                result.vertices[i] = r;
                }
            result.setOneLoop( CcwXsLoop );
            return result;
            }
        float crossSectionDiameter( const Xs& ) const {
            return 2*m_radius;
            }
    private:
        float m_radius;
        v3f m_pos;
    };

class EllipticalXsGen
    {
    public:
        explicit EllipticalXsGen( float r1, float r2, const v2f& pos=mkv2(0.0f,0.0f) ) :
            m_r1( r1 ), m_r2( r2 ), m_pos(mkv3(pos[0], pos[1])) {
            Q_ASSERT( r1 > 0 );
            Q_ASSERT( r2 > 0 );
            }

        Xs crossSection( int levelOfDetail ) const
            {
            Xs result;

            const int MinCount = 3, MaxCount = 75;
            int count = linterp( MinCount, MaxCount, levelOfDetail, MinLevelOfDetail, MaxLevelOfDetail );
            float dphi = static_cast<float>( 2*M_PI / count );
            float c = cos(dphi),   s = sin(dphi);
            M2x2 P;
            P(0,0) = P(1,1) = c;
            P(1,0) = s;
            P(0,1) = -s;
            v3f v = mkv3( 1.0f, 0.0f );
            v3fProxy2 v2 = pos2( v );
            result.vertices.resize( count+1 );
            float t = 0,   dt = 1.0f / count;
            for( int i=0; i<=count; ++i, v2=P*v2, t+=dt ) {
                vertex r;
                position( r ) = mkv3( v[0]*m_r1, v[1]*m_r2, 0.0f ) + m_pos;
                normal( r ) = mkv3( v[0]*m_r2, v[1]*m_r1, 0.0f );
                tex( r ) = mkv2( t, 0.0f );
                result.vertices[i] = r;
                }
            result.setOneLoop( CcwXsLoop );
            return result;
            }
        float crossSectionDiameter( const Xs& ) const {
            return 2*std::max( m_r1, m_r2 );
            }
    private:
        float m_r1;
        float m_r2;
        v3f m_pos;
    };

class ArcXsGen
    {
    public:
        explicit ArcXsGen( float radius, float angle, XsLoopDirection direction = CcwXsLoop, const v2f& pos=mkv2(0.0f,0.0f) ) :
            m_radius( radius ), m_angle(angle), m_direction(direction), m_pos(mkv3(pos[0], pos[1])) {
            Q_ASSERT( radius > 0 );
            }

        Xs crossSection( int levelOfDetail ) const
            {
            Xs result;

            float f = fabs(m_angle) / static_cast<float>(2*M_PI);
            float fMinCount = std::max( 3.0f*f, 1.0f ), fMaxCount = std::max( 75.0f*f, fMinCount );
            float fcount = linterp( fMinCount, fMaxCount, levelOfDetail, MinLevelOfDetail, MaxLevelOfDetail );
            int count = static_cast<int>( fcount );

            float dphi = static_cast<float>( m_angle / count );
            float c = cos(dphi),   s = sin(dphi);
            M2x2 P;
            P(0,0) = P(1,1) = c;
            P(1,0) = s;
            P(0,1) = -s;
            v3f v = mkv3( 1.0f, 0.0f );
            v3fProxy2 v2 = pos2( v );
            result.vertices.resize( count+1 );
            float t = 0,   dt = 1.0f / count;
            for( int i=0; i<=count; ++i, v2=P*v2, t+=dt ) {
                vertex r;
                position( r ) = v * m_radius + m_pos;
                normal( r ) = v;
                tex( r ) = mkv2( t, 0.0f );
                result.vertices[i] = r;
                }
            result.setOneLoop( m_direction );
            return result;
            }
        float crossSectionDiameter( const Xs& ) const {
            return 2*m_radius;
            }
    private:
        float m_radius;
        float m_angle;
        XsLoopDirection m_direction;
        v3f m_pos;
    };

class CircularTubeXsGen
    {
    public:
        CircularTubeXsGen( float innerRadius, float outerRadius, const v2f& pos=mkv2(0.0f,0.0f) ) :
            m_innerRadius( innerRadius ),
            m_outerRadius( outerRadius ),
            m_pos(mkv3(pos[0], pos[1]))
            {
            Q_ASSERT( innerRadius > 0 );
            Q_ASSERT( outerRadius > innerRadius );
            }

        Xs crossSection( int levelOfDetail ) const
            {
            Xs result;
            const int MinCount = 3, MaxCount = 75;
            int count = linterp( MinCount, MaxCount, levelOfDetail, MinLevelOfDetail, MaxLevelOfDetail );
            float dphi = static_cast<float>( 2*M_PI / count );
            float c = cos(dphi),   s = sin(dphi);
            M2x2 P;
            P(0,0) = P(1,1) = c;
            P(1,0) = s;
            P(0,1) = -s;
            v3f v = mkv3( 1.0f, 0.0f );
            v3fProxy2 v2 = pos2( v );
            result.vertices.resize( 2*(count+1) );
            float t = 0,   dt = 1.0f / count;
            for( int i=0; i<=count; ++i, v2=P*v2, t+=dt ) {
                vertex r;
                position( r ) = v * m_outerRadius + m_pos;
                normal( r ) = v;
                tex( r ) = mkv2( t, 0.0f );
                result.vertices[i] = r;
                position( r ) = v * m_innerRadius + m_pos;
                normal( r ) = -v;
                result.vertices[i+count+1] = r;
                }
            result.loops.push_back( XsLoop( mkv2(0,count+1), CcwXsLoop ) );
            result.loops.push_back( XsLoop( mkv2(count+1,2*(count+1)), CwXsLoop ) );
            return result;
            }
        float crossSectionDiameter( const Xs& ) const {
            return 2*m_outerRadius;
            }
    private:
        float m_innerRadius;
        float m_outerRadius;
        v3f m_pos;
    };

template< class XSG1, class XSG2 >
class Composite2XsGen
    {
    public:
        Composite2XsGen( const XSG1& xg1, const XSG2& xg2 ) : m_xg1( xg1 ), m_xg2( xg2 ) {}

        Xs crossSection( int levelOfDetail ) const
            {
            Xs xs1 = m_xg1.crossSection( levelOfDetail );
            Xs xs2 = m_xg2.crossSection( levelOfDetail );
            int n1 = xs1.vertices.size();
            xs1.vertices.resize( n1 + xs2.vertices.size() );
            std::copy( xs2.vertices.begin(), xs2.vertices.end(), xs1.vertices.begin() + n1 );
            xs2.loops = incIndices( xs2.loops, n1 );
            xs1.loops.splice( xs1.loops.end(), xs2.loops );
            return xs1;
            }
        float crossSectionDiameter( const Xs& xs ) const {
            // This code just estimates the diameter, not computes it preciesly.
            // Actual diameter can be greater.
            return xs.box().maxSize();
            }
    private:
        XSG1 m_xg1;
        XSG2 m_xg2;
    };

template< class XSG1, class XSG2 >
Composite2XsGen< XSG1, XSG2 > makeComposite2XsGen( const XSG1& xg1, const XSG2& xg2 ) {
    return Composite2XsGen< XSG1, XSG2 >( xg1, xg2 );
    }

class StaticXsGen
    {
    public:
        explicit StaticXsGen( const Xs& xs ) : m_xs( xs ) {}
        Xs crossSection( int /* levelOfDetail */ ) const {
            return m_xs;
            }
        float crossSectionDiameter( const Xs& xs ) const {
            return xs.box().maxSize();
            }
    private:
        Xs m_xs;
    };

} // end namespace tube_mesh

#endif // _SURF_MESH_COMMON_H_AB0B81B0_CF3E_424F_9766_BA04D388199F_
