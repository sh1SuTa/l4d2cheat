#include "͸��.h"

void ���ƿ��() {

	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	// �����ϱ߿���ɫ�ͷ�ɫ��
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 },
		ImColor(0, 153, 255) // ��ɫ
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x1 + (cheat::ActorPlayer.x2 - cheat::ActorPlayer.x1) * 0.5f, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 },
		ImColor(255, 192, 203) // ��ɫ
	);

	// ������߿���ɫ����ɫ��
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y2 },
		ImColor(0, 153, 255) // ��ɫ
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y1 + (cheat::ActorPlayer.y2 - cheat::ActorPlayer.y1) * 0.5f },
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y2 },
		ImColor(255, 255, 255) // ��ɫ
	);

	// �����ұ߿򣨷�ɫ����ɫ��
	drawList->AddLine(
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 192, 203) // ��ɫ
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 + (cheat::ActorPlayer.y2 - cheat::ActorPlayer.y1) * 0.5f },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 255, 255) // ��ɫ
	);

	// �����±߿򣨰�ɫ����ɫ��
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y2 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 255, 255) // ��ɫ
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x2 - (cheat::ActorPlayer.x2 - cheat::ActorPlayer.x1) * 0.5f, cheat::ActorPlayer.y2 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 192, 203) // ��ɫ
	);

}
bool ����2D�����С() {
	//�Ų�
	float ��������[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z };
	float ��Ļ����[2];
	if (!WorldScreen2d(��������, ��Ļ����))
	{
		return false;
	}
	//ͷ��
	float ��������2[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z + 70.f };
	float ��Ļ����2[2];
	if (!WorldScreen2d(��������2, ��Ļ����2))
	{
		return false;
	}
	cheat::ActorPlayer.x1 = ��Ļ����2[0] - (��Ļ����[1] - ��Ļ����2[1]) / 4.f;
	cheat::ActorPlayer.y1 = ��Ļ����2[1];
	cheat::ActorPlayer.x2 = ��Ļ����[0] + (��Ļ����[1] - ��Ļ����2[1]) / 4.f;
	cheat::ActorPlayer.y2 = ��Ļ����[1];
	return true;
}


void ͸�ӻ���() {
	while (true) {
		if (Menu::box == 1) {
			���ƿ��();
		}
		

	}
}
