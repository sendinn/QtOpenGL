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
	// ����ڵ����е���������еĻ���
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(processMesh(mesh, scene));
	}
	// �������������ӽڵ��ظ���һ����
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void Model::LoadModel(QString const& path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs);//���ǻ�(��Ϊdraw�ķ�ʽ��������)��UV��ת(����)��

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qDebug() << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	m_Directory = path.left(path.lastIndexOf("\\"));

	processNode(scene->mRootNode, scene);
}
