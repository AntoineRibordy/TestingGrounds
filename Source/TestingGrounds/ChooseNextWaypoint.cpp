// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BlackboardComp = OwnerComp.GetBlackboardComponent();
	Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	//UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), Index);
	if (GetPatrolPoints(OwnerComp))
	{
		if (SetNextWaypoint())
		{
			CycleIndex();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

bool UChooseNextWaypoint::GetPatrolPoints(UBehaviorTreeComponent& OwnerComp)
{
	// Get controlled pawn from AIController
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	// Get patrolpoints from AI pawn and store them
	if (!ensure(PatrolRoute)) { return false; }
	PatrolPoints = PatrolRoute->GetPatrolPoints();
	return true;
}

bool UChooseNextWaypoint::SetNextWaypoint()
{
	// Set the next waypoint in the blackboard
	if (PatrolPoints.Num() == 0) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points"));
		return false; 
	}
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	return true;
}

void UChooseNextWaypoint::CycleIndex()
{
	//Increment Index and Modulo it by #patrol points to cycle through them
	Index = (Index + 1) % PatrolPoints.Num();
	//Set the index in the blackboard to the output of the modulo
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, Index);
}