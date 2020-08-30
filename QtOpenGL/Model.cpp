#include "Model.h"


Model::Model(QString const& path)
{
	LoadModel(path);
}

Model::~Model()
{

}

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	Mesh* tmp = new Mesh();
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		ModelVertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;
		tmp->m_Vertices.push_back(vertex);
	}
	tmp->SetupMesh();
	return tmp;
}


void Model::processNode(aiNode *node, const aiScene *scene)
{
	qDebug() << node->mName.data;
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(processMesh(mesh, scene));
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void Model::LoadModel(QString const& path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs);//三角化(因为draw的方式是三角形)、UV翻转(坐标)、

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qDebug() << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	m_Directory = path.left(path.lastIndexOf("\\"));

	processNode(scene->mRootNode, scene);
}
