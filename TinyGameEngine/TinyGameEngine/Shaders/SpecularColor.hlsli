float3 CalculateSpecularColor(float3 vToL, float3 vToC, Texture2DArray texArray, SamplerState texSampler, float3 texCoord, float3 normal, float alpha)
{
    const float3 halfVector = normalize(vToL + vToC);
    const float specular = pow(max(0.0f, dot(halfVector, normal)), alpha);
    const float4 specularColor = texArray.Sample(texSampler, float3(texCoord.x, texCoord.y, texCoord.z + 1.0f));
    return specularColor * specular;
}