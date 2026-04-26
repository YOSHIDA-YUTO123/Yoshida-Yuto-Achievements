//===================================================
//
// 頂点バッファのデータ [vertex_buffer_component.hpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _VERTEX_BUFFER_COMPONENT_HPP_
#define _VERTEX_BUFFER_COMPONENT_HPP_

//***************************************************
// 頂点バッファのコンポーネントの定義
//***************************************************
struct VertexBufferComponent
{
	VertexBufferComponent() :
		pVtxBuffer(nullptr), pIdxBuffer(nullptr) {}

	// コピー禁止
	VertexBufferComponent(const VertexBufferComponent&) = delete;
	VertexBufferComponent& operator=(const VertexBufferComponent&) = delete;

	~VertexBufferComponent()
	{
		// 頂点バッファの破棄
		if (pVtxBuffer != nullptr)
		{
			pVtxBuffer->Release();
			pVtxBuffer = nullptr;
		}

		// インデックスバッファの破棄
		if (pIdxBuffer != nullptr)
		{
			pIdxBuffer->Release();
			pIdxBuffer = nullptr;
		}
	}

	LPDIRECT3DVERTEXBUFFER9 pVtxBuffer; // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuffer;	// インデックスバッファへのポインタ
};
#endif