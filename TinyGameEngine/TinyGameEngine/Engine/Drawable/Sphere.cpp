#include "Sphere.h"
#include "../Bindable/BindableHeader.h"

#define M_PI 3.14159265358979323846

Sphere::Sphere(Graphics& gfx, float radius)
{
    pos = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    if (!IsStaticInitialized())
    {
        std::vector<DirectX::XMFLOAT3> vertices;
        std::vector<unsigned int> indices;
        createSphere(radius, 30, 30, vertices, indices);

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

        auto pvs = std::make_unique<VertexShader>(gfx, L"SolidVS.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"SolidPS.cso"));



        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
        };
        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexBufferFromStatic();
    }

    AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this, 0));
}

Sphere::~Sphere()
{
}

void Sphere::SetPos(DirectX::XMFLOAT4 p)
{
    pos = p;
}

void Sphere::Update(float dt)
{

}

DirectX::XMMATRIX Sphere::GetModelMatrix() const
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void Sphere::createSphere(float radius, int numLatitudeLines, int numLongitudeLines,
    std::vector<DirectX::XMFLOAT3>& vertices, std::vector<unsigned int>& indices) {
    // Clear the vectors
    vertices.clear();
    indices.clear();

    // Calculate increments
    float phiInc = M_PI / (numLatitudeLines + 1);
    float thetaInc = 2 * M_PI / numLongitudeLines;

    // North pole vertex
    vertices.push_back({ 0, 0, radius });

    // Generate vertices with spherical coordinates
    for (int i = 1; i <= numLatitudeLines; ++i) {
        float phi = phiInc * i;
        for (int j = 0; j < numLongitudeLines; ++j) {
            float theta = thetaInc * j;
            vertices.push_back({ radius * sin(phi) * cos(theta),
                                radius * sin(phi) * sin(theta),
                                radius * cos(phi) });
        }
    }

    // South pole vertex
    vertices.push_back({ 0, 0, -radius });

    // Generate indices for the top cap
    for (int i = 0; i < numLongitudeLines; ++i) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2 == numLongitudeLines + 1 ? 1 : i + 2);
    }

    // Generate indices for the middle area
    for (int i = 0; i < numLatitudeLines - 1; ++i) {
        for (int j = 0; j < numLongitudeLines; ++j) {
            int current = i * numLongitudeLines + j + 1;
            int next = current + numLongitudeLines;

            // Triangle 1
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(j + 1 == numLongitudeLines ? (i * numLongitudeLines + 1) : current + 1);

            // Triangle 2
            indices.push_back(current + 1 == (i + 1) * numLongitudeLines + 1 ? (i * numLongitudeLines + 1) : current + 1);
            indices.push_back(next);
            indices.push_back(next + 1 == ((i + 1) * numLongitudeLines + 1) ? ((i + 1) * numLongitudeLines + 1) : next + 1);
        }
    }

    // Generate indices for the bottom cap
    int lastVertexIndex = (int)vertices.size() - 1;
    for (int i = 0; i < numLongitudeLines; ++i) {
        indices.push_back(lastVertexIndex);
        indices.push_back(lastVertexIndex - i - 1);
        indices.push_back(lastVertexIndex - i - 2 == lastVertexIndex - numLongitudeLines - 1 ? lastVertexIndex - 1 : lastVertexIndex - i - 2);
    }
}