// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrollingGuard.h" // TODO remove coupling
#include "AIController.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BlackboardComp = OwnerComp.GetBlackboardComponent();
	Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	//UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), Index);
	GetPatrolPoints(OwnerComp);
	SetNextWaypoint();
	CycleIndex();
	return EBTNodeResult::Succeeded;
}

void UChooseNextWaypoint::GetPatrolPoints(UBehaviorTreeComponent& OwnerComp)
{
	// Get controlled pawn from AIController
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();
	// Get patrolpoints from AI pawn and store them
	PatrolPointsCPP = Cast<APatrollingGuard>(ControlledPawn)->PatrolPointsCPP;
}

void UChooseNextWaypoint::SetNextWaypoint()
{
	// Set the next waypoint in the blackboard
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPointsCPP[Index]);
}

void UChooseNextWaypoint::CycleIndex()
{
	//Increment Index
	Index++;
	//Modulo the index by #patrol points to cycle through them
	Index = Index % PatrolPointsCPP.Num();
	//Set the index in the blackboard to the output of the modulo
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, Index);
}