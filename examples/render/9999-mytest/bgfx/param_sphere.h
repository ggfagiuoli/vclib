#include <bx/math.h>
#include <bx/bx.h>
#include <vector>

struct SphereMesh{
    std::vector<bx::Vec3> vertices;
    std::vector<bx::Vec3> normals;
    std::vector<uint16_t> indices;
};

class ParametricSphereGenerator {
        float radius;
    public:
        ParametricSphereGenerator(float r){
            radius = r;
        }

        void setRadius(float r){
            radius=r;
        }

        float getRadius(){
            return radius;
        }

        ParametricSphereGenerator(ParametricSphereGenerator &other){
            this->radius = other.radius;
        }

        ParametricSphereGenerator& operator=(ParametricSphereGenerator& other){
            this->radius = other.radius;
            return *this;
        }

        //theta in [0,2pi); phi in [0,pi]
        bx::Vec3 getPoint(float theta, float phi){
            return bx::Vec3{radius*bx::cos(theta)*bx::sin(phi), radius*bx::sin(theta)*bx::sin(phi), radius*bx::cos(phi)};
        }

        //Il numero di campioni totali è esattamente samplesS*(samplesT-2)+2. Questo perchè la funzione parametrica di sopra restituisce
        //sempre lo stesso valore per phi=0 e phi=pi indipendentemente da theta (quindi verrebbero tanti punti uguali...)
        SphereMesh toMesh(uint8_t samplesS, uint8_t samplesT){
            samplesS = bx::max(3, samplesS);
            samplesT = bx::max(3, samplesT);
            std::vector<bx::Vec3> vertices;
            std::vector<bx::Vec3> normals;
            std::vector<uint16_t> indices;
            uint16_t currentIndex = 1;
            const uint16_t maxRow = (samplesT-2);
            const uint16_t maxCol = samplesS;
            vertices.push_back(getPoint(0.f,0.f));
            for(uint16_t currRow = 0; currRow<maxRow; currRow++){
                float curT = bx::kPi/(float)(samplesT-1)*(currRow+1);
                for(uint16_t currCol = 0; currCol<maxCol; currCol++){
                    float curS = bx::kPi2/(float)samplesS*currCol;
                    bx::Vec3 point = getPoint(curS,curT);
                    normals.push_back(bx::div(point, radius));
                    vertices.push_back(point);
                    uint16_t nextPointIndex = currCol+1==maxCol?samplesS*currRow+1:currentIndex+1;
                    uint16_t prevPointIndex = currCol==0?samplesS*currRow+maxCol:currentIndex-1;
                    uint16_t pointAboveIndex = currRow==0?0:currentIndex-samplesS; 
                    uint16_t pointBelowIndex = currRow+1==maxRow?samplesS*(samplesT-2)+1:currentIndex+samplesS;
                    indices.insert(indices.end(), {currentIndex, nextPointIndex, pointAboveIndex});
                    indices.insert(indices.end(), {currentIndex, prevPointIndex, pointBelowIndex});
                    currentIndex++;
                }
            }
            vertices.push_back(getPoint(0.f,bx::kPi));
            return SphereMesh{vertices, normals, indices};
        };
};